// Fill out your copyright notice in the Description page of Project Settings.

#include "MashButtonInput.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerInput.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UMashButtonInput::UMashButtonInput()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	// Default keys
	Keys.Add(FKey("Spacebar"));
	Keys.Add(FKey("Gamepad_FaceButton_Bottom"));

	// Set default vaules for mashing 
	MashValueNeeded = 1.0F;
	Difficulty = EDifficultyLevel::DL_Normal;

}

UGameplayStatics::FinishSpawningActor()

// Called when the game starts
void UMashButtonInput::BeginPlay()
{
	Super::BeginPlay();

	
}


// Called every frame
void UMashButtonInput::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	
}

void UMashButtonInput::SetKeys(const FName InputAction, bool bOverrideKeys)
{
	if (bOverrideKeys)
	{
		Keys.Empty(); // Clear the array if necessary
	}

		InputActionToUse = InputAction;
		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		TArray<FInputActionKeyMapping> FoundKeys = PC->PlayerInput->GetKeysForAction(InputAction);


}

