// Fill out your copyright notice in the Description page of Project Settings.

#include "TargetSpawnGroup.h"
#include "BounceTarget.h"

// Sets default values
ATargetSpawnGroup::ATargetSpawnGroup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATargetSpawnGroup::BeginPlay()
{
	Super::BeginPlay();

	/*for (int i = 0; i < TargetSpawners.Num(); ++i) {
		TargetSpawners[i]->SetSpawnerGroupRef(this);
	}*/
	
	// Spawn InitialSpawnAmnt amount of targets
	// delay used, because event can't be triggered same frame as BeginPlay, as that is when the events are bound
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ATargetSpawnGroup::SpawnInitialTargets);
}

// Called every frame
void ATargetSpawnGroup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SpawnTimer -= DeltaTime;

	if (KilledTargets > StopKillCount) return;
	else if (KilledTargets == StopKillCount) {
		KilledTargets++;
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ATargetSpawnGroup::ResetSpawners, ResetDelay, false);
		return;
	}

	if (CurrentTargets >= MaxTargets || SpawnTimer >= 0.0f) return;
	SpawnTarget();
	SpawnTimer = SpawnRate;
}

void ATargetSpawnGroup::SpawnTarget()
{
	// Get random spawner, then broadcast spawn target event with that
	// spawner pointer as a param
	int randomIndex = GetRandomIndexFromArray(TargetSpawners);

	if (randomIndex == -1) return;
	if (TargetSpawners[randomIndex] == nullptr) return;
	TargetSpawners[randomIndex]->SpawnRandomTarget();
	CurrentTargets++;
}

void ATargetSpawnGroup::SpawnInitialTargets()
{
	for (int i = 0; i < InitialSpawnAmnt; ++i) {
		SpawnTarget();
	}
}

void ATargetSpawnGroup::ResetSpawners()
{
	KilledTargets = 0;
}

int ATargetSpawnGroup::GetRandomIndexFromArray(const TArray<ATargetSpawner*>& Array)
{
	if (Array.IsEmpty()) return -1; // Return -1 if invalid

	int RandomIndex = FMath::RandRange(0, Array.Num() - 1);
	return RandomIndex;
}

void ATargetSpawnGroup::TargetKillHandler()
{
	KilledTargets++;
	CurrentTargets--;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("KilledTargets: %i / %i"), KilledTargets, StopKillCount));
}