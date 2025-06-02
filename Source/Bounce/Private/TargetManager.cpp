// Fill out your copyright notice in the Description page of Project Settings.

#include "TargetManager.h"
#include "EventDispatcher.h"
#include "TargetSpawner.h"

// Sets default values
ATargetManager::ATargetManager() {}

// Called when the game starts or when spawned
void ATargetManager::BeginPlay()
{
	Super::BeginPlay();

	UEventDispatcher::GetEventManagerSingleton()->Event_TargetKill.AddUniqueDynamic(this, &ATargetManager::TargetKillHandler);
}

void ATargetManager::TargetKillHandler()
{
	//SpawnTarget();

	KilledTargets++;

	if (!WaveData.IsValidIndex(CurrentWave)) return;

	// If amount of targets killed has hit the 
	// wave threshhold, advance to next wave
	if (KilledTargets < WaveData[CurrentWave].WaveThreshold) return;

	uint8 Len = TargetSpawners.Num();
	for (uint8 i = 0; i < Len; ++i) {
		if (TargetSpawners[i] == nullptr) return;
		TargetSpawners[i]->NewSpawnWeights(WaveData[CurrentWave]);
	}

	CurrentWave++;
}

int ATargetManager::GetRandomIndexFromArray(const TArray<ATargetSpawner*>& Array)
{
	if (Array.IsEmpty()) return -1; // Return -1 if invalid

	int RandomIndex = FMath::RandRange(0, Array.Num() - 1);
	return RandomIndex;
}