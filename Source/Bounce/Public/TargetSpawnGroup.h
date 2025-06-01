// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TargetSpawner.h"
#include "TargetSpawnGroup.generated.h"

UCLASS()
class BOUNCE_API ATargetSpawnGroup : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Spawners")
	int InitialSpawnAmnt = 10;

	UPROPERTY(EditAnywhere, Category = "Spawners")
	int MaxTargets = 20;

	UPROPERTY(EditAnywhere, Category = "Spawners")
	int TargetsToSpawn = 20;

	int CurrentTargets = 0;

	int KilledTargets = 0;

	UPROPERTY(EditAnywhere, Category = "Spawners")
	float InitialSpawnDelay = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Spawners")
	float SpawnRate = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Spawners")
	float SpawnTimer = InitialSpawnDelay;

	UPROPERTY(EditAnywhere, Category = "Spawners")
	float ResetDelay = 600.0f;

	UPROPERTY(EditAnywhere, Category = "Spawners")
	TArray<ATargetSpawner*> TargetSpawners;

public:
	
public:	
	// Sets default values for this actor's properties
	ATargetSpawnGroup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void TargetKillHandler();

	UFUNCTION()
	void SpawnTarget();

	UFUNCTION()
	void SpawnInitialTargets();

	UFUNCTION()
	void ResetSpawners();

	int GetRandomIndexFromArray(const TArray<ATargetSpawner*>& Array);
};
