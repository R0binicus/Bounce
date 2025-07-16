// Fill out your copyright notice in the Description page of Project Settings.

#include "RespawnTriggerArea.h"
#include "PlayerCharacter.h"
#include "Components/BoxComponent.h"

// Sets default values
ARespawnTriggerArea::ARespawnTriggerArea() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Area Box"));
	BoxComp->SetBoxExtent(FVector(1000, 1000, 1000), false);
}

// Called when the game starts or when spawned
void ARespawnTriggerArea::BeginPlay() {
	Super::BeginPlay();
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ARespawnTriggerArea::OverlapBegin);
}

// Called every frame
void ARespawnTriggerArea::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void ARespawnTriggerArea::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor == nullptr) return;
	if (OtherActor == this) return;
	if (OtherComp == nullptr) return;

	if (OtherComp->GetCollisionProfileName() != "Pawn") return;

	APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);

	if (player) {
		player->Respawn();
	}
}