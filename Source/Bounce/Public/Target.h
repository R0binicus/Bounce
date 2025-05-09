// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Target.generated.h"

class UBoxComponent;

UCLASS()
class BOUNCE_API ATarget : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ATarget();

	/** Box collision component */
	UPROPERTY(VisibleDefaultsOnly, Category = Target)
	UBoxComponent* CollisionComponent;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly, Category = "Target")
	float MaxHealth;

	UPROPERTY(EditDefaultsOnly, Category = "Target")
	int Score = 1;

	UPROPERTY()
	float CurrentHealth;
};