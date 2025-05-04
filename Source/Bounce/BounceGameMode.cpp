// Copyright Epic Games, Inc. All Rights Reserved.

#include "BounceGameMode.h"
#include "BounceCharacter.h"
#include "UObject/ConstructorHelpers.h"


#include "Kismet/GameplayStatics.h"
#include "BounceTarget.h"

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

	TargetTimer -= DeltaTime;

	if (TargetTimer < 0.0f)
	{
		TargetTimer = 0.5f;

		UWorld* world = GetWorld();

		if (world)
		{
			int playerIndex = 0;

			FVector playerLocation = UGameplayStatics::GetPlayerCharacter(GetWorld(), playerIndex)->GetActorLocation();

			FVector targetLocation = playerLocation;

			targetLocation.X += FMath::RandRange(-1000.0f, 1000.0f);
			targetLocation.Y += FMath::RandRange(-1000.0f, 1000.0f);

			ABounceTarget* enemy = world->SpawnActor<ABounceTarget>(TargetBlueprint, targetLocation, FRotator::ZeroRotator);
		}
	}
}

void ABounceGameMode::BeginPlay()
{
	Super::BeginPlay();
}
