// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BounceTarget.h"
#include "TargetSpawner.generated.h"

UCLASS()
class BOUNCE_API ATargetSpawner : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Spawn")
	TArray<TSubclassOf<class ABounceTarget>> WaveSpawnWeights;

	float SpawningAreaSizeX = 1000.0f;
	float SpawningAreaSizeY = 1000.0f;
	float SpawningAreaSizeZ = 0.0f;

	float TargetTimer;

public:	
	/** Box component */
	UPROPERTY(VisibleDefaultsOnly, Category = Spawn)
	UBoxComponent* BoxComp;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	TArray<TSubclassOf<class ABounceTarget>> TargetBlueprints;

public:
	// Sets default values for this actor's properties
	ATargetSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void NewTargetHandler(ATargetSpawner* Spawner);

	UFUNCTION()
	void NewSpawnWeights(int target1, int target2, int target3, int target4, int target5, int target6, int target7, int target8, int target9);

	ABounceTarget* SpawnTarget(FVector targetLocation, UWorld* world);

	int GetRandomIndexFromArray(const TArray<TSubclassOf<class ABounceTarget>>& Array);
};
