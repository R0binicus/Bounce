// Fill out your copyright notice in the Description page of Project Settings.

#include "TargetSpawnGroup.h"
#include "PlayerCharacter.h"
#include "BounceTarget.h"
#include "Components/BoxComponent.h"

// Sets default values
ATargetSpawnGroup::ATargetSpawnGroup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerCheckAreaBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Player Check Area Box"));
	PlayerCheckAreaBox->SetBoxExtent(FVector(3000, 3000, 3000), false);
}

// Called when the game starts or when spawned
void ATargetSpawnGroup::BeginPlay()
{
	Super::BeginPlay();

	PlayerCheckAreaBox->OnComponentBeginOverlap.AddDynamic(this, &ATargetSpawnGroup::OverlapBegin);
	PlayerCheckAreaBox->OnComponentEndOverlap.AddDynamic(this, &ATargetSpawnGroup::OverlapEnd);

	for (int i = 0; i < TargetSpawners.Num(); ++i) {
		TargetSpawners[i]->SetSpawnerGroupRef(this);
	}
}

// Called every frame
void ATargetSpawnGroup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ResetTimerActive) {
		ResetTimer -= DeltaTime;
		if (ResetTimer <= 0) {
			ResetSpawners();
		}
	}

	SpawnTimer -= DeltaTime;

	if (KilledTargets > StopKillCount) return;
	else if (KilledTargets == StopKillCount) {
		KilledTargets++;
		FTimerHandle TimerHandle;
		ResetTimerActive = true;
		ResetTimer = ResetDelay;
		return;
	}

	if (CurrentTargets >= MaxTargets || SpawnTimer >= 0.0f) return;
	SpawnTimer = SpawnRate;
	if (!PlayerInArea) return;
	SpawnTarget();
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

void ATargetSpawnGroup::ResetSpawners_Implementation()
{
	KilledTargets = 0;
	ResetTimerActive = false;
}

int ATargetSpawnGroup::GetRandomIndexFromArray(const TArray<ATargetSpawner*>& Array)
{
	if (Array.IsEmpty()) return -1; // Return -1 if invalid

	int RandomIndex = FMath::RandRange(0, Array.Num() - 1);
	return RandomIndex;
}

void ATargetSpawnGroup::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor == nullptr) return;
	if (OtherActor == this) return;
	if (OtherComp == nullptr) return;
	if (OtherComp->GetCollisionProfileName() != "Pawn") return;

	PlayerInArea = true;
}
void ATargetSpawnGroup::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (OtherActor == nullptr) return;
	if (OtherActor == this) return;
	if (OtherComp == nullptr) return;
	if (OtherComp->GetCollisionProfileName() != "Pawn") return;

	PlayerInArea = false;
}

void ATargetSpawnGroup::TargetKillHandler_Implementation()
{
	KilledTargets++;
	CurrentTargets--;
}