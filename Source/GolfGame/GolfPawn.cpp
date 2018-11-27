// Fill out your copyright notice in the Description page of Project Settings.

#include "GolfPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GolfGame/MashButtonInput.h"
#include "UObject/ConstructorHelpers.h"


#define Print(x) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT(x));};
//#define PrintWithBool(x, var)if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT(x "%s"), var ? TEXT("True") : TEXT("False"))); }
#define PrintWithBool(x, var)if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT(x "%s"), var ? TEXT("True") : TEXT("False"))); }
#define PrintWithFloat(x, var) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, (FString(x) + FString::SanitizeFloat(var)));};

// Sets default values
AGolfPawn::AGolfPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Allow the player to hit the ball at start of game
	bCanHitBall = true;

	CameraSpeedMultiplier = 20.0F;

	// CurveFloat for determining impulse
	auto ImpulseCurveAsset = ConstructorHelpers::FObjectFinder<UCurveFloat>(TEXT("CurveFloat'/Game/GolfGame/Core/Movement/FImpulseAmount_Default.FImpulseAmount_Default'"));
	ImpulseAmountCurve = ImpulseCurveAsset.Object;

	// Create ball collision sphere collision component
	BallCollision = CreateDefaultSubobject<USphereComponent>(TEXT("BallCollision"));
	BallCollision->SetSphereRadius(2.1335F);
	BallCollision->SetCollisionProfileName(TEXT("PhysicsActor"));
	BallCollision->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	BallCollision->SetSimulatePhysics(true);

	// Create ball static mesh component
	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
	BallMesh->SetupAttachment(BallCollision);
	BallMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Create camera spring arm component
	BallCameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	BallCameraSpringArm->SetupAttachment(BallCollision);
	//BallCameraSpringArm->SetRelativeRotation(FRotator(-20.0F, 0.0F, 0.0F));
	BallCameraSpringArm->bUsePawnControlRotation = true;
	BallCameraSpringArm->TargetArmLength = 100.0F;
	BallCameraSpringArm->SocketOffset = FVector(0.0F, 0.0F, -20.0F);
	BallCameraSpringArm->bEnableCameraLag = true;
	BallCameraSpringArm->CameraLagSpeed = 3.0F;

	// Create camera component
	BallCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("BallCamera"));
	BallCamera->SetupAttachment(BallCameraSpringArm, USpringArmComponent::SocketName);

	// Create mash button input
	MashInput = CreateDefaultSubobject<UMashButtonInput>(TEXT("MashInput"));

}

// Called when the game starts or when spawned
void AGolfPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGolfPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGolfPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind functions to input events  &AGolfPawn::OnStartBallCharge
	PlayerInputComponent->BindAction("StartBallHit", IE_Pressed, this, &AGolfPawn::OnPressBallCharge);
	PlayerInputComponent->BindAction("StartBallHit", IE_Released, this, &AGolfPawn::OnReleaseBallCharge);
	PlayerInputComponent->BindAxis("CameraUp", this, &AGolfPawn::OnCameraUp);
	PlayerInputComponent->BindAxis("CameraRight", this, &AGolfPawn::OnCameraRight);

}

void AGolfPawn::OnPressBallCharge()
{
	if (bCanHitBall)
	{
		bIsChargingHit = true;

		PrintWithBool("Is Charging: ", bIsChargingHit);
	}
}

void AGolfPawn::OnReleaseBallCharge()
{
	if (bCanHitBall)
	{

		ImpulseInput = FMath::Clamp(ImpulseInput, 0.0F, 1.0F);
		PrintWithFloat("Impulse Input, Clamped: ", ImpulseInput);

		ImpulseInput = ImpulseAmountCurve->GetFloatValue(ImpulseInput);
		PrintWithFloat("Impulse Input, Adjusted With Curve: ", ImpulseInput);

		BallCollision->AddImpulse(BallCamera->GetForwardVector() * ImpulseInput);

		// Reset values for next hit
		bIsChargingHit = false;
		ImpulseInput = 0.0F;

		PrintWithBool("Is Charging: ", bIsChargingHit);
	}
}

void AGolfPawn::OnCameraUp(float AxisValue)
{
	if (!bIsChargingHit && AxisValue != 0)
	{
		// Apply AxisValue to camera pitch
		AddControllerPitchInput(AxisValue * GetWorld()->GetDeltaSeconds() * CameraSpeedMultiplier * -1.0F);
	}
}

void AGolfPawn::OnCameraRight(float AxisValue)
{
	if (AxisValue != 0)
	{
		if (bIsChargingHit)
		{
			// Apply AxisValue to HitChargeValue, adjusted for framerate
			ImpulseInput += (AxisValue * GetWorld()->GetDeltaSeconds());
		}
		else
		{
			// Otherwise, apply AxisValue to camera yaw
			AddControllerYawInput(AxisValue * GetWorld()->GetDeltaSeconds() * CameraSpeedMultiplier);
		}
	}
}

