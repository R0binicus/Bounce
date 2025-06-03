// Fill out your copyright notice in the Description page of Project Settings.

#include "BounceTarget.h"
#include "DrawDebugHelpers.h"
#include "Components/BoxComponent.h"
#include "EventDispatcher.h"
#include "Kismet/GameplayStatics.h"
#include "ScorePopup.h"
#include "TargetSpawnGroup.h"
#include "TargetSpawner.h"
#include "Projectile.h"

// Sets default values
ABounceTarget::ABounceTarget()
{
	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	CollisionComp->BodyInstance.SetCollisionProfileName("Target");
	CollisionComp->OnComponentHit.AddDynamic(this, &ABounceTarget::OnHit);		// set up a notification for when this component hits something blocking

	RootComponent = CollisionComp;
}

// Called when the game starts or when spawned
void ABounceTarget::BeginPlay()
{
	Super::BeginPlay();

	//Initialize the target's Health
	CurrentHealth = MaxHealth;
}

void ABounceTarget::Death(AActor* OtherActor)
{
	IsDead = true;
	SetLifeSpan(CorpseTime);
	UGameplayStatics::PlaySoundAtLocation(this, KillSound, GetActorLocation());
	CollisionComp->SetCollisionProfileName("DeadTarget");
	CollisionComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore); // Or Overlap, Ignore etc.
	CollisionComp->SetSimulatePhysics(true);
	CollisionComp->AddRadialImpulse(OtherActor->GetActorLocation(), 1000.f, 1000.f, ERadialImpulseFalloff::RIF_Linear, true);
	UEventDispatcher::GetEventManagerSingleton()->Event_TargetKill.Broadcast();
	UEventDispatcher::GetEventManagerSingleton()->Event_AddScore.Broadcast(Score);

	if (Spawner == nullptr) return;
	if (Spawner->SpawnGroup == nullptr) return;
	Spawner->SpawnGroup->TargetKillHandler();
}

void ABounceTarget::SetSpawnerRef(ATargetSpawner* NewSpawner)
{
	if (NewSpawner == nullptr) return;
	Spawner = NewSpawner;
}

void ABounceTarget::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor == nullptr) return;
	if (OtherActor == this) return;
	if (OtherComp == nullptr) return;
	if (OtherComp->GetCollisionProfileName() == "OldProjectile") return;
	if (AProjectile* projectile = Cast<AProjectile>(OtherActor)) {
		CurrentHealth = CurrentHealth - projectile->GetProjectileDamage();
		if (CurrentHealth > 0) {
			UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
		} 
		else {
			Death(OtherActor);
		}
	}
}