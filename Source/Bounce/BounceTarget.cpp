// Fill out your copyright notice in the Description page of Project Settings.

#include "BounceTarget.h"
#include "DrawDebugHelpers.h"
#include "Components/BoxComponent.h"

// Sets default values
ABounceTarget::ABounceTarget()
{
	MyCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	MyCollisionBox->SetCollisionProfileName("Target");

	RootComponent = MyCollisionBox;

	MyCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ABounceTarget::OnComponentBeginOverlap);
}

void ABounceTarget::OnComponentBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		Destroy();
	}
}


