// Fill out your copyright notice in the Description page of Project Settings.

#include "Target.h"
#include "DrawDebugHelpers.h"
#include "Components/BoxComponent.h"
#include "EventDispatcher.h"
#include "Projectile.h"

// Sets default values
ATarget::ATarget()
{
	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	CollisionComponent->BodyInstance.SetCollisionProfileName("Target");
	CollisionComponent->OnComponentHit.AddDynamic(this, &ATarget::OnHit);		// set up a notification for when this component hits something blocking

	RootComponent = CollisionComponent;
}

// Called when the game starts or when spawned
void ATarget::BeginPlay()
{
	Super::BeginPlay();

	//Initialize the target's Health
	CurrentHealth = MaxHealth;
}

void ATarget::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if((OtherActor != nullptr) && (OtherActor != this) && (OtherComponent != nullptr))
	{
		if(OtherComponent->GetCollisionProfileName() != "Projectile") return;
		if(AProjectile* projectile = Cast<AProjectile>(OtherActor))
		{
			CurrentHealth = CurrentHealth-projectile->GetProjectileDamage();
			if(CurrentHealth > 0) return;

			UEventDispatcher::GetEventManagerSingleton()->Event_TargetKill.Broadcast();
			UEventDispatcher::GetEventManagerSingleton()->Event_AddScore.Broadcast(Score);
            Destroy();
		}
	}
}