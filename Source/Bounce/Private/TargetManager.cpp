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

	// If amount of targets killed has hit the 
	// wave threshhold, advance to next wave
	if (KilledTargets < WaveThresholds[CurrentWave]) return;

	if (CurrentWave == 0) {
		EventManager->Event_WaveWeights.Broadcast(1, 1, 0, 0, 0, 0, 0, 0, 0);
	}
	else if (CurrentWave == 1) {
		EventManager->Event_WaveWeights.Broadcast(1, 2, 1, 0, 0, 0, 0, 0, 0);
	}
	else if (CurrentWave == 2) {
		EventManager->Event_WaveWeights.Broadcast(0, 1, 2, 1, 0, 0, 0, 0, 0);
	}
	else if (CurrentWave == 3) {
		EventManager->Event_WaveWeights.Broadcast(0, 1, 1, 1, 1, 1, 0, 0, 0);
	}
	else if (CurrentWave == 4) {
		EventManager->Event_WaveWeights.Broadcast(0, 0, 1, 1, 2, 2, 0, 0, 0);
	}
	else if (CurrentWave == 5) {
		EventManager->Event_WaveWeights.Broadcast(0, 0, 0, 1, 1, 1, 1, 1, 0);
	}
	else if (CurrentWave == 6) {
		EventManager->Event_WaveWeights.Broadcast(0, 0, 0, 0, 1, 1, 1, 1, 1);
	}
	else return;
	CurrentWave++;
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