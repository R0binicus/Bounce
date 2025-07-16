// Fill out your copyright notice in the Description page of Project Settings.

#include "CheckpointArea.h"
#include "PlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"

// Sets default values
ACheckpointArea::ACheckpointArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneCompRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneCompRoot"));
	RootComponent = SceneCompRoot;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Area Box"));
	BoxComp->SetBoxExtent(FVector(1000, 1000, 1000), false);
	BoxComp->SetupAttachment(RootComponent); //Make it follow root component

	SceneCompRespawn = CreateDefaultSubobject<USceneComponent>(TEXT("RespawnPosition"));
	SceneCompRespawn->SetupAttachment(RootComponent); //Make it follow root component
}

// Called when the game starts or when spawned
void ACheckpointArea::BeginPlay()
{
	Super::BeginPlay();

	RespawnPosition = SceneCompRespawn->GetComponentLocation();
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ACheckpointArea::OverlapBegin);
}

// Called every frame
void ACheckpointArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACheckpointArea::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == nullptr) return;
	if (OtherActor == this) return;
	if (OtherComp == nullptr) return;
	//FName ProfileName = OtherComp->GetCollisionProfileName();
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Collision Profile Name: %s"), ProfileName));
	if (OtherComp->GetCollisionProfileName() != "Pawn") return;

	APlayerCharacter* TempChar = Cast<APlayerCharacter>(OtherActor);

	if (TempChar)
	{
		TempChar->SetRespawnPosition(RespawnPosition);
	}
}

