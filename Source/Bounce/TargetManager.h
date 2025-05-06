// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TargetSpawner.h"
#include "TargetManager.generated.h"

UCLASS()
class BOUNCE_API ATargetManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATargetManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float InitialSpawnDelay;

	int GetRandomIndexFromArray(const TArray<ATargetSpawner*>& Array);

	UFUNCTION()
	void TargetKillHandler();

	UFUNCTION()
	void SpawnTarget();

	UPROPERTY(EditAnywhere, Category = "Spawners")
	int InitialSpawnAmnt = 10;

	UPROPERTY(EditAnywhere, Category = "Spawners")
	int Wave2Amnt = 10;

	UPROPERTY(EditAnywhere, Category = "Spawners")
	int MaxTargets = 10;

	int CurrentTargets;

	int KilledTargets;

	int CurrentWave;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Spawners")
	TArray<ATargetSpawner*> TargetSpawners;
};
