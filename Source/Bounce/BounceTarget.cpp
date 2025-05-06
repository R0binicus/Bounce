// Fill out your copyright notice in the Description page of Project Settings.

#include "BounceTarget.h"
#include "DrawDebugHelpers.h"
#include "Components/BoxComponent.h"
#include "EventDispatcher.h"
#include "BounceProjectile.h"

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

void ABounceTarget::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		if (OtherComp->GetCollisionProfileName() != "Projectile") return;
		if (ABounceProjectile* projectile = Cast<ABounceProjectile>(OtherActor))
		{
			CurrentHealth = CurrentHealth - projectile->GetProjectileDamage();
			if (CurrentHealth > 0) return;
			Destroy();
			UEventDispatcher::GetEventManagerSingleton()->Event_TargetKill.Broadcast();
		}
	}
}