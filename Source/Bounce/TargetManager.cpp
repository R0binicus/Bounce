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
	GameTimer = 1.0f;

	CurrentWave = 0;
	CurrentTargets = 0;
	KilledTargets = 0;
	UEventDispatcher::GetEventManagerSingleton()->Event_TargetKill.AddUniqueDynamic(this, &ATargetManager::TargetKillHandler);
	
}

// Called every frame
void ATargetManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GameTimer >= 1.0f)
	{
		GameTimer = 0.0f;
		TargetKillHandler();
		TargetKillHandler();
		TargetKillHandler();
		TargetKillHandler();
		TargetKillHandler();
		TargetKillHandler();
		TargetKillHandler();
		TargetKillHandler();
		TargetKillHandler();
		TargetKillHandler();
	}
}

int ATargetManager::GetRandomIndexFromArray(const TArray<ATargetSpawner*>& Array)
{
	if (Array.IsEmpty())
	{
		return -1; // Return -1 or handle empty array case as needed
	}

	int RandomIndex = FMath::RandRange(0, Array.Num() - 1);
	return RandomIndex;
}

void ATargetManager::TargetKillHandler()
{
	KilledTargets++;
	if (CurrentWave == 0 && KilledTargets >= Wave2Amnt)
	{
		CurrentWave = 1;
		UEventDispatcher::GetEventManagerSingleton()->Event_WaveWeights.Broadcast(1, 1);
	}

	int randomIndex = GetRandomIndexFromArray(TargetSpawners);

	if (randomIndex == -1 || TargetSpawners[randomIndex] == nullptr) return;
	UEventDispatcher::GetEventManagerSingleton()->Event_SpawnTarget.Broadcast(TargetSpawners[randomIndex]);
}