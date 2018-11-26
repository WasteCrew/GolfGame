// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MashButtonInput.generated.h"

// Control over how difficult the game will feel
UENUM(BlueprintType)
enum class EDifficultyLevel : uint8
{
	DL_Easy UMETA(DisplayName = "Easy"),
	DL_Normal UMETA(DisplayName = "Normal"),
	DL_Hard UMETA(DisplayName = "Hard"),
	DL_Expert UMETA(DisplayName = "Expert")
};

// Called when the mash event ends, which has been started by calling StartMashing
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMashEnded, bool, bPassedEvent, float, FinalMashValue);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GOLFGAME_API UMashButtonInput : public UActorComponent
{
	GENERATED_BODY()

public:	
	//Sets default values for this component's properties
	UMashButtonInput();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


private:
	/** Decrement and increment, if correct key pressed, MashValue, per frame
	*/
	void AdjustMashValue(float DeltaTime);

protected:

	// Whether to check for key input events each frame
	UPROPERTY(BlueprintReadWrite, Category="Mash")
	bool bEnableMashing;

	// The base amount to decrease MashValue per tick (x DeltaTime)
	const float MashDownAmount = 10.0F;

	// The base amount to decrease MashValue per tick (x DeltaTime)
	const float MashUpAmount = 50.0F;

	// Control how much each difficulty level will decrease MashValue by per tick
	TArray<float> MashDownMult_Difficulty;

	// Reference to the player controller
	APlayerController* PCRef;
	
	// Attempt to return a valid player controller. Will set PCRef so can be easily gotten next time.
	APlayerController* TryGetPC();


public:

	/** Set the keys which mashing will react to.
	* @param InputAction	The input action to get key values from
	* @param bOverrideKeys	Optionally, don't remove previous contents of Keys array
	*/
	UFUNCTION(BlueprintCallable, Category="Mash")
	void SetKeys(const FName InputAction, bool bOverrideKeys = true);

	// Which keys are accepted for this mash event
	UPROPERTY(BlueprintReadOnly, Category="Mash")
	TArray<FKey> Keys;

protected:

	// Whether or not the mashing should continue indefinitely, or until told to stop
	UPROPERTY(BlueprintReadWrite, Category="Mash")
	bool bEndless = false;

public:
	/** Start a mash event if other variables are set and ready to go
	* @param MashTime			The duration of this mash event in seconds. If <= 0, no timer will be set and mash event will be stopped upon reaching MashValueToPass or if manually stopped.
	* @param MashValueToPass	The value required in order to pass this mash event
	*/
	UFUNCTION(BlueprintCallable, Category = "Mash")
	void StartMashing(float MashValueToPass, float MashTime);

	/** Used to make this component stop listening for buttons mashes, when bEndless is true
	*/
	UFUNCTION(BlueprintCallable, Category = "Mash")
	void StopMashing();

	// Called when the mash event ends, which has been started by calling StartMashing
	UPROPERTY(BlueprintAssignable, Category = "Mash")
	FOnMashEnded OnMashEnded;



	// The current value of Mashing
	UPROPERTY(BlueprintReadOnly, Category = "Mash")
	float MashValue;

	// The mash value needed to pass the mash event
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mash")
	float MashValueNeeded;

	// Controls how much MashValue will decrease each frame
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mash")
	EDifficultyLevel Difficulty;

};
