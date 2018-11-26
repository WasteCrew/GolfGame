// Fill out your copyright notice in the Description page of Project Settings.

#include "MashButtonInput.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerInput.h"
#include "Kismet/GameplayStatics.h"

#define Print(x) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, x);};
#define PrintWithBool(x, var)if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT(x "%s"), var ? TEXT("True") : TEXT("False"))); }
#define PrintWithFloat(x, var) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, (FString(x) + FString::SanitizeFloat(var)));};


// Sets default values for this component's properties
UMashButtonInput::UMashButtonInput()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	// Default keys
	Keys.Add(FKey("Spacebar"));
	Keys.Add(FKey("Gamepad_LeftShoulder"));

	// Set default vaules for mashing 
	MashValueNeeded = 30.0F;
	Difficulty = EDifficultyLevel::DL_Normal;

	MashDownMult_Difficulty.Add(0.1F);
	MashDownMult_Difficulty.Add(0.5F);
	MashDownMult_Difficulty.Add(1.0F);
	MashDownMult_Difficulty.Add(1.5F);

}

// Called when the game starts
void UMashButtonInput::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void UMashButtonInput::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//	PrintWithBool("Mashing Enabled: ", bEnableMashing);
	//	PrintWithBool("Keys Has Keys: ", Keys.IsValidIndex(0));
	//	PrintWithBool("Player Controller Valid: ", TryGetPC());
	//	PrintWithBool("Just pressed correct key: ", TryGetPC()->WasInputKeyJustPressed(FKey("Spacebar")));
	//Print(FString::SanitizeFloat(MashDownMult_Difficulty[uint8(Difficulty)]));

	// Ensure we are allowed to mash and Keys isn't empty and player controller is valid
	if (bEnableMashing && Keys.IsValidIndex(0) && TryGetPC())
	{
		AdjustMashValue(DeltaTime);

		// Stop mashing if reached required MashValue
		if (!bEndless && MashValue > MashValueNeeded)
		{
			bEnableMashing = false; // Stop checking for buttons every frame
			MashValue = MashValueNeeded; // Set to exactly MashValueNeeded to look a bit neater
		}
		//PrintWithFloat("Mash Value: ", MashValue);
	}

}

void UMashButtonInput::AdjustMashValue(float DeltaTime)
{
	// Reduce MashValue, adjusted for frame time and difficulty
	const float DecrAmnt = (MashDownAmount * DeltaTime * MashDownMult_Difficulty[uint8(Difficulty)]);
	MashValue -= DecrAmnt;
	MashValue = (MashValue < 0) ? 0.0F : MashValue;  // Ensure MashValue doesn't go below 0

	// Increase MashValue if correct key was pressed, adjusted for frame time
	const float IncrAmnt = MashUpAmount * DeltaTime;
	for (FKey Key : Keys) // Check all available correct keys to see if any were pressed
	{
		bool bIncrVal = TryGetPC()->WasInputKeyJustPressed(Key); // Check if this key was pressed
		if (bIncrVal)
		{
			MashValue += IncrAmnt;
			break; // Break at first correct key so that user can't get double MashValue for pressing keyboard and gamepad
		}
	}

}

APlayerController * UMashButtonInput::TryGetPC()
{
	if (!PCRef)
	{
		PCRef = GetWorld()->GetFirstPlayerController(); // Set PCRef if it was not valid
	}

	return PCRef; // Return PCRef
}

void UMashButtonInput::SetKeys(const FName InputAction, bool bOverrideKeys)
{
	if (bOverrideKeys)
	{
		Keys.Empty(); // Clear the array if necessary
	}

	// Use PlayerInput to check key bindings associated with inputted InputAction
	//APlayerController* PC = GetWorld()->GetFirstPlayerController();
	TArray<FInputActionKeyMapping> FoundKeyMappings = TryGetPC()->PlayerInput->GetKeysForAction(InputAction);

	for (FInputActionKeyMapping KeyMapping : FoundKeyMappings)
	{
		Keys.Add(KeyMapping.Key); // Add each found key to Keys
	}

}

void UMashButtonInput::StartMashing(float MashValueToPass, float MashTime)
{
	if (Keys.IsValidIndex(0) && TryGetPC())
	{
		MashValue = 0.0F; // Reset MashValue
		MashValueNeeded = MashValueToPass;

		// Start changing MashValue each frame
		bEnableMashing = bEndless = true;

		// Set the timer to stop the mash event if necessary
		if (MashTime > 0)
		{
			FTimerHandle MashTimer;
			GetWorld()->GetTimerManager().SetTimer(MashTimer, this, &UMashButtonInput::StopMashing, MashTime, false);
		}
	}
}

void UMashButtonInput::StopMashing()
{
	// Stop changing MashValue each frame
	bEnableMashing = bEndless = false;
	OnMashEnded.Broadcast(MashValue>=MashValueNeeded, MashValue); // Broadcast results
}
