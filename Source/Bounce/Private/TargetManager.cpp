// Fill out your copyright notice in the Description page of Project Settings.

#include "TargetManager.h"
#include "EventDispatcher.h"
#include "TargetSpawner.h"

// Sets default values
ATargetManager::ATargetManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	EventManager = UEventDispatcher::GetEventManagerSingleton();
}

// Called when the game starts or when spawned
void ATargetManager::BeginPlay()
{
	Super::BeginPlay();

	EventManager->Event_TargetKill.AddUniqueDynamic(this, &ATargetManager::TargetKillHandler);

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

	// If 1st wave and amount of targets killed has hit the 
	// threshhold, advance to wave 2
	if (CurrentWave == 0 && KilledTargets >= WaveThresholds[0]) {
		CurrentWave++;
		EventManager->Event_WaveWeights.Broadcast(1, 1, 0, 0, 0, 0, 0, 0, 0);
	}
	else if (CurrentWave == 1 && KilledTargets >= WaveThresholds[1]) {
		CurrentWave++;
		EventManager->Event_WaveWeights.Broadcast(1, 2, 1, 0, 0, 0, 0, 0, 0);
	}
	else if (CurrentWave == 2 && KilledTargets >= WaveThresholds[2]) {
		CurrentWave++;
		EventManager->Event_WaveWeights.Broadcast(0, 1, 2, 1, 0, 0, 0, 0, 0);
	}
	else if (CurrentWave == 3 && KilledTargets >= WaveThresholds[3]) {
		CurrentWave++;
		EventManager->Event_WaveWeights.Broadcast(0, 1, 1, 1, 1, 1, 0, 0, 0);
	}
	else if (CurrentWave == 4 && KilledTargets >= WaveThresholds[4]) {
		CurrentWave++;
		EventManager->Event_WaveWeights.Broadcast(0, 0, 1, 1, 2, 2, 0, 0, 0);
	}
	else if (CurrentWave == 5 && KilledTargets >= WaveThresholds[5]) {
		CurrentWave++;
		EventManager->Event_WaveWeights.Broadcast(0, 0, 0, 1, 1, 1, 1, 1, 0);
	}
	else if (CurrentWave == 6 && KilledTargets >= WaveThresholds[6]) {
		CurrentWave++;
		EventManager->Event_WaveWeights.Broadcast(0, 0, 0, 0, 1, 1, 1, 1, 1);
	}
}

void ATargetManager::SpawnTarget()
{
	// Get random spawner, then broadcast spawn target event with that
	// spawner pointer as a param
	int randomIndex = GetRandomIndexFromArray(TargetSpawners);

	if (randomIndex == -1) return;
	if (TargetSpawners[randomIndex] == nullptr) return;
	EventManager->Event_SpawnTarget.Broadcast(TargetSpawners[randomIndex]);
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