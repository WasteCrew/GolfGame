// Fill out your copyright notice in the Description page of Project Settings.

#include "GolfGameGameModeBase.h"
#include "GolfPawn.h"

AGolfGameGameModeBase::AGolfGameGameModeBase(const FObjectInitializer& ObjectInitializer)
{
	//Super(ObjectInitializer);

	// Set default classes
	DefaultPawnClass = AGolfPawn::StaticClass();
}