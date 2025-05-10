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
	
public:	
	// Sets default values for this actor's properties
	ATargetSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void NewTargetHandler(ATargetSpawner* Spawner);

	ABounceTarget* SpawnTarget(FVector targetLocation, UWorld* world);

	UPROPERTY(EditAnywhere, Category = "Spawn")
	TArray<TSubclassOf<class ABounceTarget>> WaveSpawnWeights;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	float SpawningAreaSizeX = 1000.0f;
	UPROPERTY(EditAnywhere, Category = "Spawn")
	float SpawningAreaSizeY = 1000.0f;
	UPROPERTY(EditAnywhere, Category = "Spawn")
	float SpawningAreaSizeZ = 0.0f;

	int GetRandomIndexFromArray(const TArray<TSubclassOf<class ABounceTarget>>& Array);

	UFUNCTION()
	void NewSpawnWeights(int target1, int target2);

	float TargetTimer;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	TArray<TSubclassOf<class ABounceTarget>> TargetBlueprints;
};
