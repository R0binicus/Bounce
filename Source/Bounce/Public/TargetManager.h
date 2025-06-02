// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TargetSpawner.h"
#include "TargetWaveData.h"
#include "TargetManager.generated.h"

UCLASS()
class BOUNCE_API ATargetManager : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = Spawners)
	TArray<FTargetWaveData> WaveData;

	int KilledTargets = 0;

	int CurrentWave = 0;

	UPROPERTY(EditAnywhere, Category = "Spawners")
	TArray<ATargetSpawner*> TargetSpawners;

public:
	ATargetManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void TargetKillHandler();

	int GetRandomIndexFromArray(const TArray<ATargetSpawner*>& Array);
};
