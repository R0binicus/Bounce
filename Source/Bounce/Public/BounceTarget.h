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

public:	
	// Sets default values for this actor's properties
	ABounceTarget();

	/** Box collision component */
	UPROPERTY(BlueprintReadWrite, Category = Target)
	UBoxComponent* CollisionComp;

	UPROPERTY(BlueprintReadWrite, Category = Target)
	bool IsDead = false;

	/** Sound to play when we die */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	USoundBase* KillSound;

	/** Sound to play when we die */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	USoundBase* HitSound;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly, Category = "Target")
	float MaxHealth;

	UPROPERTY(EditDefaultsOnly, Category = "Target")
	int Score = 1;

	UPROPERTY()
	float CurrentHealth;

	UPROPERTY(EditDefaultsOnly, Category = "Target")
	float CorpseTime = 5.f;
};
