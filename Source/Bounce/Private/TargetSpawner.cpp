// Fill out your copyright notice in the Description page of Project Settings.

#include "TargetSpawner.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "BounceTarget.h"
#include "PlayerCharacter.h"

// Sets default values
ATargetSpawner::ATargetSpawner()
{
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnVolume Box"));
	BoxComp->SetBoxExtent(FVector(1000, 1000, 0), false);
}

// Called when the game starts or when spawned
void ATargetSpawner::BeginPlay()
{
	Super::BeginPlay();

	FVector boxExtents = BoxComp->GetUnscaledBoxExtent();

	SpawningAreaSizeX = boxExtents.X;
	SpawningAreaSizeY = boxExtents.Y;
	SpawningAreaSizeZ = boxExtents.Z;

	// Default target spawning (wave one)
	FTargetWaveData waveData;
	waveData.WaveAmounts.Add(DefaultTargetSpawn, 1);
	NewSpawnWeights(waveData);
}

void ATargetSpawner::SetSpawnerGroupRef(ATargetSpawnGroup* NewSpawnerGroup)
{
	SpawnGroup = NewSpawnerGroup;
}

void ATargetSpawner::SpawnRandomTarget()
{
	UWorld* world = GetWorld();

	if (!world) return;

	// Set random location variation
	FVector targetLocation = GetActorLocation();

	ABounceTarget* target = nullptr;

	// Attempt to spawn target 10 times before giving up to prevent crash
	int32 i = 0;
	while (i < 10) {
		target = SpawnTarget(targetLocation, world);
		if (target != nullptr) {
			return;
		}
		else {
			i++;
		}
	}
	UE_LOG(LogClass, Error, TEXT("Target failed to spawn: %i"), i);
}

ABounceTarget* ATargetSpawner::SpawnTarget(FVector targetLocation, UWorld* world)
{
	targetLocation.X += FMath::RandRange(-SpawningAreaSizeX, SpawningAreaSizeX);
	targetLocation.Y += FMath::RandRange(-SpawningAreaSizeY, SpawningAreaSizeY);
	targetLocation.Z += FMath::RandRange(-SpawningAreaSizeZ, SpawningAreaSizeZ);
	FRotator targetRotation = FRotator(0.0f, FMath::RandRange(0.0f, 360.0f), 0.0f);

	// Get random target type from weights, then spawn target
	int randomIndex = GetRandomIndexFromArray(WaveSpawnWeights);

	FActorSpawnParameters ActorSpawnParameters;
	ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;

	if (randomIndex == -1) return nullptr;
	if (WaveSpawnWeights[randomIndex] == nullptr) return nullptr;
	ABounceTarget* target = world->SpawnActor<ABounceTarget>(WaveSpawnWeights[randomIndex], targetLocation, targetRotation, ActorSpawnParameters);
	TempTarget = target;
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ATargetSpawner::SetTargetValues);

	return target;
}

void ATargetSpawner::SetTargetValues()
{
	TempTarget->SetSpawnerRef(this);
}

// Set WaveSpawnWeights to be empty, then add the target types
// back into the array based on the input weights
void ATargetSpawner::NewSpawnWeights(FTargetWaveData waveData)
{
	WaveSpawnWeights.SetNum(0);

	// Iterate over the TargetWaveData's TargetAmounts,
	// in each target-weight pair, add the target (key) to the WaveSpawnWeights
	// an amount equal to the weight (value)
	for (auto& targetWeight : waveData.WaveAmounts) {
		uint8 Len = targetWeight.Value;
		for (uint8 i = 0; i < Len; ++i) {
			WaveSpawnWeights.Add(targetWeight.Key);
		}
	}
}

int ATargetSpawner::GetRandomIndexFromArray(const TArray<TSubclassOf<class ABounceTarget>>& Array)
{
	if (Array.IsEmpty()) return -1; // Return -1 if invalid

	int RandomIndex = FMath::RandRange(0, Array.Num() - 1);
	return RandomIndex;
}
