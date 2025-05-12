// Fill out your copyright notice in the Description page of Project Settings.

#include "TargetSpawner.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "BounceTarget.h"
#include "PlayerCharacter.h"
#include "EventDispatcher.h"

// Sets default values
ATargetSpawner::ATargetSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnVolume Box"));
	BoxComp->SetBoxExtent(FVector(1000, 1000, 0), false);
}

// Called when the game starts or when spawned
void ATargetSpawner::BeginPlay()
{
	Super::BeginPlay();

	// Bind events
	UEventDispatcher::GetEventManagerSingleton()->Event_SpawnTarget.AddUniqueDynamic(this, &ATargetSpawner::NewTargetHandler);
	UEventDispatcher::GetEventManagerSingleton()->Event_WaveWeights.AddUniqueDynamic(this, &ATargetSpawner::NewSpawnWeights);

	FVector boxExtents = BoxComp->GetUnscaledBoxExtent();

	SpawningAreaSizeX = boxExtents.X;
	SpawningAreaSizeY = boxExtents.Y;
	SpawningAreaSizeZ = boxExtents.Z;

	NewSpawnWeights(1, 0, 0, 0, 0, 0, 0, 0, 0);
}

// Called every frame
void ATargetSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*TargetTimer -= DeltaTime;

	if (TargetTimer < 0.0f)
	{
		TargetTimer = 0.5f;

		UWorld* world = GetWorld();

		if (world)
		{
			int playerIndex = 0;

			FVector targetLocation = GetActorLocation();

			targetLocation.X += FMath::RandRange(-SpawningAreaWidth, SpawningAreaWidth);
			targetLocation.Y += FMath::RandRange(-SpawningAreaWidth, SpawningAreaWidth);

			int randomIndex = GetRandomIndexFromArray(WaveSpanWeights);

			if (randomIndex == -1 || WaveSpanWeights[randomIndex] == nullptr) return;
			ABounceTarget* enemy = world->SpawnActor<ABounceTarget>(WaveSpanWeights[randomIndex], targetLocation, FRotator::ZeroRotator);
		}
	}*/
}

void ATargetSpawner::NewTargetHandler(ATargetSpawner* Spawner)
{
	if (Spawner != this) return;

	UWorld* world = GetWorld();

	if (!world) return;

	// Set random location variation
	FVector targetLocation = GetActorLocation();

	ABounceTarget* target = nullptr;

	// Attempt to spawn target 10 times before giving up to prevent crash
	int32 i = 0;
	while (i < 10)
	{
		target = SpawnTarget(targetLocation, world);
		if (target != nullptr)
		{
			return;
		}
		else
		{
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

	if (randomIndex == -1 || WaveSpawnWeights[randomIndex] == nullptr) return nullptr;
	ABounceTarget* target = world->SpawnActor<ABounceTarget>(WaveSpawnWeights[randomIndex], targetLocation, targetRotation, ActorSpawnParameters);

	return target;
}

int ATargetSpawner::GetRandomIndexFromArray(const TArray<TSubclassOf<class ABounceTarget>>& Array)
{
	if (Array.IsEmpty())
	{
		return -1; // Return -1 if invalid
	}

	int RandomIndex = FMath::RandRange(0, Array.Num() - 1);
	return RandomIndex;
}

// Set WaveSpawnWeights to be empty, then add the target types
// back into the array based on the input weights
void ATargetSpawner::NewSpawnWeights(int target1, int target2, int target3, int target4, int target5, int target6, int target7, int target8, int target9)
{
	WaveSpawnWeights.SetNum(0);

	if (TargetBlueprints.IsValidIndex(0) && TargetBlueprints[0] != nullptr)
	{
		uint8 Len = target1;
		for (uint8 i = 0; i < Len; ++i)
		{
			WaveSpawnWeights.Add(TargetBlueprints[0]);
		}
	}

	if (TargetBlueprints.IsValidIndex(1) && TargetBlueprints[1] != nullptr)
	{
		uint8 Len = target2;
		for (uint8 i = 0; i < Len; ++i)
		{
			WaveSpawnWeights.Add(TargetBlueprints[1]);
		}
	}

	if (TargetBlueprints.IsValidIndex(2) && TargetBlueprints[2] != nullptr)
	{
		uint8 Len = target3;
		for (uint8 i = 0; i < Len; ++i)
		{
			WaveSpawnWeights.Add(TargetBlueprints[2]);
		}
	}

	if (TargetBlueprints.IsValidIndex(3) && TargetBlueprints[3] != nullptr)
	{
		uint8 Len = target4;
		for (uint8 i = 0; i < Len; ++i)
		{
			WaveSpawnWeights.Add(TargetBlueprints[3]);
		}
	}

	if (TargetBlueprints.IsValidIndex(4) && TargetBlueprints[4] != nullptr)
	{
		uint8 Len = target5;
		for (uint8 i = 0; i < Len; ++i)
		{
			WaveSpawnWeights.Add(TargetBlueprints[4]);
		}
	}

	if (TargetBlueprints.IsValidIndex(5) && TargetBlueprints[5] != nullptr)
	{
		uint8 Len = target6;
		for (uint8 i = 0; i < Len; ++i)
		{
			WaveSpawnWeights.Add(TargetBlueprints[5]);
		}
	}

	if (TargetBlueprints.IsValidIndex(6) && TargetBlueprints[6] != nullptr)
	{
		uint8 Len = target7;
		for (uint8 i = 0; i < Len; ++i)
		{
			WaveSpawnWeights.Add(TargetBlueprints[6]);
		}
	}

	if (TargetBlueprints.IsValidIndex(7) && TargetBlueprints[7] != nullptr)
	{
		uint8 Len = target8;
		for (uint8 i = 0; i < Len; ++i)
		{
			WaveSpawnWeights.Add(TargetBlueprints[7]);
		}
	}

	if (TargetBlueprints.IsValidIndex(8) && TargetBlueprints[8] != nullptr)
	{
		uint8 Len = target9;
		for (uint8 i = 0; i < Len; ++i)
		{
			WaveSpawnWeights.Add(TargetBlueprints[8]);
		}
	}
}
