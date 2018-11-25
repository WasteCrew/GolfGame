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

protected:

	// Whether to check for key input events each frame
	bool bEnableMashing;

	float MashValue;


	// The base amount to decrease MashValue per tick (x DeltaTime)
	const float MashDownAmount = 10.0F;

	// Control for how much each difficulty level will decrease MashValue per tick
	const float MashDownMult_Easy = 0.1F;
	const float MashDownMult_Normal = 0.5F;
	const float MashDownMult_Hard = 1.0F;
	const float MashDownMult_Expert = 1.5F;

public:

	void SetKeys(const FName InputAction, bool bOverrideKeys = true);

	// Which keys are accepted for this mash event
	TArray<FKey> Keys;

	FName InputActionToUse;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mash")
	float MashValueNeeded;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mash")
	EDifficultyLevel Difficulty;

};
