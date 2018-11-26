// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GolfPawn.generated.h"

UCLASS()
class GOLFGAME_API AGolfPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGolfPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


protected:
	UPROPERTY(EditAnywhere, Category = "Components")
	class USpringArmComponent* BallCameraSpringArm;
	UPROPERTY(EditAnywhere, Category = "Components")
	class UCameraComponent* BallCamera;
	UPROPERTY(EditAnywhere, Category = "Components")
	class USphereComponent* BallCollision;
	UPROPERTY(EditAnywhere, Category = "Components")
	class UStaticMeshComponent* BallMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class UMashButtonInput* MashInput;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Golf")
	bool bCanHitBall = true;

protected:
	// Input variables and functions

	// Whether or not the player is currently charging their ball hit
	bool bIsChargingHit;

	// Bound to input events
	void OnPressBallCharge();
	void OnReleaseBallCharge();
	void OnCameraUp(float AxisValue);
	void OnCameraRight(float AxisValue);

};
