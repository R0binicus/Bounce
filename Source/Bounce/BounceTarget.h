// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BounceTarget.generated.h"

class UBoxComponent;

UCLASS()
class BOUNCE_API ABounceTarget : public AActor
{
	GENERATED_BODY()

	/** Box collision component */
	UPROPERTY(VisibleDefaultsOnly, Category = Target)
	UBoxComponent* MyCollisionBox;

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Sets default values for this actor's properties
	ABounceTarget();
};
