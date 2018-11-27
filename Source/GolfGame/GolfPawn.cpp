// Fill out your copyright notice in the Description page of Project Settings.

#include "GolfPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GolfGame/MashButtonInput.h"

#define Print(x) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT(x));};
//#define PrintWithBool(x, var)if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT(x "%s"), var ? TEXT("True") : TEXT("False"))); }
#define PrintWithBool(x, var)if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT(x "%s"), var ? TEXT("True") : TEXT("False"))); }

// Sets default values
AGolfPawn::AGolfPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Allow the player to hit the ball at start of game
	bCanHitBall = true;

	// Range of ball hit impulses
	BallImpulseMin = 3.0F;
	BallImpulseMax = 15.0F;

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
	BallCameraSpringArm->SetRelativeLocationAndRotation((FVector(0.0F, 0.0F, 20.0F)), FRotator(-20.0F, 0.0F, 0.0F));
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
		bIsChargingHit = false;
		PrintWithBool("Is Charging: ", bIsChargingHit);
		BallCollision->AddImpulse(GetActorForwardVector() * BallImpulseMult);
	}
}

void AGolfPawn::OnCameraUp(float AxisValue)
{
	if (AxisValue != 0)
	{
		Print("Cam Change Up");
	}
}

void AGolfPawn::OnCameraRight(float AxisValue)
{
	if (AxisValue != 0)
	{
		Print("Cam Change Right");
	}
}

