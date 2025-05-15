// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TargetSpawner.h"
#include "EventDispatcher.h"
#include "TargetWaveData.h"
#include "TargetManager.generated.h"

UCLASS()
class BOUNCE_API ATargetManager : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Spawners")
	int InitialSpawnAmnt = 10;

	UPROPERTY(EditAnywhere, Category = Spawners)
	TArray<FTargetWaveData> WaveData;

	UPROPERTY(EditAnywhere, Category = "Spawners")
	int MaxTargets = 20;

	int CurrentTargets = 0;

	int KilledTargets = 0;

	int CurrentWave = 0;

	UEventDispatcher* EventManager;

	UPROPERTY(EditAnywhere, Category = "Spawners")
	float InitialSpawnDelay = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Spawners")
	float SpawnRate = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Spawners")
	float SpawnTimer = InitialSpawnDelay;

	UPROPERTY(EditAnywhere, Category = "Spawners")
	TArray<ATargetSpawner*> TargetSpawners;

public:
	ATargetManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void TargetKillHandler();

	UFUNCTION()
	void SpawnTarget();

	UFUNCTION()
	void SpawnInitialTargets();

	int GetRandomIndexFromArray(const TArray<ATargetSpawner*>& Array);
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
