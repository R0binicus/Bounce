// Fill out your copyright notice in the Description page of Project Settings.

#include "TargetManager.h"
#include "EventDispatcher.h"
#include "TargetSpawner.h"

// Sets default values
ATargetManager::ATargetManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATargetManager::BeginPlay()
{
	Super::BeginPlay();

	UEventDispatcher::GetEventManagerSingleton()->Event_TargetKill.AddUniqueDynamic(this, &ATargetManager::TargetKillHandler);
}

// Called every frame
void ATargetManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 1 Second after game starts, spawn InitialSpawnAmnt amount of targets
	// delay used, because event can't be triggered in BeginPlay, as that is when the events are bound
	if (InitialSpawnDelay >= 1.0f)
	{
		InitialSpawnDelay = 0.0f;

		for (int i = 0; i < InitialSpawnAmnt; ++i)
		{
			SpawnTarget();
		}
	}

	SpawnTimer -= DeltaTime;

	if (CurrentTargets < MaxTargets && SpawnTimer < 0.0f)
	{
		SpawnTarget();
		SpawnTimer = SpawnRate;
	}
}

int ATargetManager::GetRandomIndexFromArray(const TArray<ATargetSpawner*>& Array)
{
	if (Array.IsEmpty())
	{
		return -1; // Return -1 if invalid
	}

	int RandomIndex = FMath::RandRange(0, Array.Num() - 1);
	return RandomIndex;
}

void ATargetManager::TargetKillHandler()
{
	SpawnTarget();

	KilledTargets++;
	// If 1st wave and amount of targets killed has hit the 
	// threshhold, advance to wave 2
	if (CurrentWave == 0 && KilledTargets >= WaveThresholds[0])
	{
		CurrentWave = 1;
		UEventDispatcher::GetEventManagerSingleton()->Event_WaveWeights.Broadcast(1, 1, 0, 0, 0, 0);
	}
	else if (CurrentWave == 1 && KilledTargets >= WaveThresholds[1])
	{
		CurrentWave = 2;
		UEventDispatcher::GetEventManagerSingleton()->Event_WaveWeights.Broadcast(1, 2, 1, 0, 0, 0);
	}
	else if (CurrentWave == 2 && KilledTargets >= WaveThresholds[2])
	{
		CurrentWave = 3;
		UEventDispatcher::GetEventManagerSingleton()->Event_WaveWeights.Broadcast(0, 1, 2, 1, 0, 0);
	}
	else if (CurrentWave == 3 && KilledTargets >= WaveThresholds[3])
	{
		CurrentWave = 4;
		UEventDispatcher::GetEventManagerSingleton()->Event_WaveWeights.Broadcast(0, 1, 1, 1, 1, 1);
	}
	else if (CurrentWave == 4 && KilledTargets >= WaveThresholds[4])
	{
		CurrentWave = 5;
		UEventDispatcher::GetEventManagerSingleton()->Event_WaveWeights.Broadcast(0, 0, 1, 1, 2, 2);
	}
}

void ATargetManager::SpawnTarget()
{
	// Get random spawner, then broadcast spawn target event with that
	// spawner pointer as a param
	int randomIndex = GetRandomIndexFromArray(TargetSpawners);

	if (randomIndex == -1 || TargetSpawners[randomIndex] == nullptr) return;
	UEventDispatcher::GetEventManagerSingleton()->Event_SpawnTarget.Broadcast(TargetSpawners[randomIndex]);
	CurrentTargets++;
}