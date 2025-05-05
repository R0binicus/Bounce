// Copyright Epic Games, Inc. All Rights Reserved.

#include "BounceGameMode.h"
#include "BounceCharacter.h"
#include "UObject/ConstructorHelpers.h"

ABounceGameMode::ABounceGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	PrimaryActorTick.bCanEverTick = true;
}

void ABounceGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABounceGameMode::BeginPlay()
{
	Super::BeginPlay();
}
