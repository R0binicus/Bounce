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

	// Spawn InitialSpawnAmnt amount of targets
	// delay used, because event can't be triggered same frame as BeginPlay, as that is when the events are bound
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ATargetManager::SpawnInitialTargets);
}

// Called every frame
void ATargetManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SpawnTimer -= DeltaTime;

	if (CurrentTargets >= MaxTargets || SpawnTimer >= 0.0f) return;
		SpawnTarget();
		SpawnTimer = SpawnRate;
}

void ATargetManager::TargetKillHandler()
{
	SpawnTarget();

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

void ATargetManager::SpawnTarget()
{
	// Get random spawner, then broadcast spawn target event with that
	// spawner pointer as a param
	int randomIndex = GetRandomIndexFromArray(TargetSpawners);

	if (randomIndex == -1) return;
	if (TargetSpawners[randomIndex] == nullptr) return;
	TargetSpawners[randomIndex]->SpawnRandomTarget();
	CurrentTargets++;
}

void ATargetManager::SpawnInitialTargets()
{
	for (int i = 0; i < InitialSpawnAmnt; ++i) {
		SpawnTarget();
	}
}

int ATargetManager::GetRandomIndexFromArray(const TArray<ATargetSpawner*>& Array)
{
	if (Array.IsEmpty()) return -1; // Return -1 if invalid

	int RandomIndex = FMath::RandRange(0, Array.Num() - 1);
	return RandomIndex;
}