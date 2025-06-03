// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BounceTarget.h"
#include "TargetWaveData.h"
#include "TargetSpawner.generated.h"

class ABounceTarget;

UCLASS()
class BOUNCE_API ATargetSpawner : public AActor
{
	GENERATED_BODY()

protected:
	/** Box component */
	UPROPERTY(VisibleDefaultsOnly, Category = Spawn)
	UBoxComponent* BoxComp;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	TSubclassOf<class ABounceTarget> DefaultTargetSpawn;

	TArray<TSubclassOf<class ABounceTarget>> WaveSpawnWeights;

	float SpawningAreaSizeX = 1000.0f;
	float SpawningAreaSizeY = 1000.0f;
	float SpawningAreaSizeZ = 0.0f;

	float TargetTimer;
	ABounceTarget* TempTarget;

public:
	UPROPERTY(EditAnywhere, Category = "Spawn")
	ATargetSpawnGroup* SpawnGroup = nullptr;

public:
	// Sets default values for this actor's properties
	ATargetSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	ABounceTarget* SpawnTarget(FVector targetLocation, UWorld* world);

	UFUNCTION()
	void SetTargetValues();

	int GetRandomIndexFromArray(const TArray<TSubclassOf<class ABounceTarget>>& Array);

public:
	UFUNCTION()
	void SetSpawnerGroupRef(ATargetSpawnGroup* NewSpawner);

	UFUNCTION()
	void SpawnRandomTarget();

	UFUNCTION()
	void NewSpawnWeights(FTargetWaveData waveData);
};
