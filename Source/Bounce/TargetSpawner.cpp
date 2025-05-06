// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetSpawner.h"
#include "Kismet/GameplayStatics.h"
#include "BounceTarget.h"
#include "BounceCharacter.h"
#include "EventDispatcher.h"

// Sets default values
ATargetSpawner::ATargetSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATargetSpawner::BeginPlay()
{
	Super::BeginPlay();

	UEventDispatcher::GetEventManagerSingleton()->Event_SpawnTarget.AddUniqueDynamic(this, &ATargetSpawner::SpawnTargetHandler);
	UEventDispatcher::GetEventManagerSingleton()->Event_WaveWeights.AddUniqueDynamic(this, &ATargetSpawner::NewSpawnWeights);

	NewSpawnWeights(1, 0);
}

// Called every frame
void ATargetSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*TargetTimer -= DeltaTime;

	if (TargetTimer < 0.0f)
	{
		TargetTimer = 0.5f;

		UWorld* world = GetWorld();

		if (world)
		{
			int playerIndex = 0;

			FVector targetLocation = GetActorLocation();

			targetLocation.X += FMath::RandRange(-1000.0f, 1000.0f);
			targetLocation.Y += FMath::RandRange(-1000.0f, 1000.0f);

			int randomIndex = GetRandomIndexFromArray(WaveSpanWeights);

			if (randomIndex == -1 || WaveSpanWeights[randomIndex] == nullptr) return;
			ABounceTarget* enemy = world->SpawnActor<ABounceTarget>(WaveSpanWeights[randomIndex], targetLocation, FRotator::ZeroRotator);
		}
	}*/
}

void ATargetSpawner::SpawnTargetHandler()
{
	UWorld* world = GetWorld();

	if (world)
	{
		int playerIndex = 0;

		FVector targetLocation = GetActorLocation();

		targetLocation.X += FMath::RandRange(-1000.0f, 1000.0f);
		targetLocation.Y += FMath::RandRange(-1000.0f, 1000.0f);

		int randomIndex = GetRandomIndexFromArray(TargetBlueprints);

		if (randomIndex == -1 || TargetBlueprints[randomIndex] == nullptr) return;
		ABounceTarget* enemy = world->SpawnActor<ABounceTarget>(TargetBlueprints[randomIndex], targetLocation, FRotator::ZeroRotator);
	}
}

int ATargetSpawner::GetRandomIndexFromArray(const TArray<TSubclassOf<class ABounceTarget>>& Array)
{
	if (Array.IsEmpty())
	{
		return -1; // Return -1 or handle empty array case as needed
	}

	int RandomIndex = FMath::RandRange(0, Array.Num() - 1);
	return RandomIndex;
}

void ATargetSpawner::NewSpawnWeights(int target1, int target2)
{
	WaveSpawnWeights.SetNum(0);

	if (TargetBlueprints.IsValidIndex(0) && TargetBlueprints[0] != nullptr)
	{
		uint8 Len = target1;
		for (uint8 i = 0; i < Len; ++i)
		{
			WaveSpawnWeights.Add(TargetBlueprints[0]);
		}
	}

	if (TargetBlueprints.IsValidIndex(1) && TargetBlueprints[1] != nullptr)
	{
		uint8 Len = target2;
		for (uint8 i = 0; i < Len; ++i)
		{
			WaveSpawnWeights.Add(TargetBlueprints[1]);
		}
	}
}
