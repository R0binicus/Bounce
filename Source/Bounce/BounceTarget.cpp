// Fill out your copyright notice in the Description page of Project Settings.

#include "BounceTarget.h"
#include "DrawDebugHelpers.h"
#include "Components/BoxComponent.h"
#include "EventDispatcher.h"

// Sets default values
ABounceTarget::ABounceTarget()
{
	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	CollisionComp->BodyInstance.SetCollisionProfileName("Target");
	CollisionComp->OnComponentHit.AddDynamic(this, &ABounceTarget::OnHit);		// set up a notification for when this component hits something blocking

	RootComponent = CollisionComp;
}

void ABounceTarget::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		if (OtherComp->GetCollisionProfileName() != "Projectile") return;
		UEventDispatcher::GetEventManagerSingleton()->Event_TargetKill.Broadcast();
		Destroy();
	}
}