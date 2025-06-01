// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UStaticMeshComponent;
class UProjectileMovementComponent;

UCLASS(config=Game)
class BOUNCE_API AProjectile : public AActor
{
	GENERATED_BODY()

private:
	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	UStaticMeshComponent* CollisionComponent;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Movement, meta=(AllowPrivateAccess="true"))
	UProjectileMovementComponent* MovementComponent;

protected:
	// Damage dealt to hit actor on collision start
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	float Damage = 10.0f;

	// Maximum number of bounces before the projectile is destroyed
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	int Bounces = 5;

	// Maximum number of bounces before the projectile bounces off targets
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	int ElderlyBounce = 3;

	// Delay before projectile can hit the player
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	float HitPlayerDelay = 1.0f;

	// Size of projectile
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	FVector Scale = FVector(1.f, 1.f, 1.f);

	/** Sound to play each time we bounce */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	USoundBase* BounceSound;

public:
	AProjectile();

	/** Returns CollisionComp subobject **/
	UStaticMeshComponent* GetCollisionComp() const { return CollisionComponent; }

	/** Returns ProjectileMovement subobject **/
	UProjectileMovementComponent* GetProjectileMovement() const { return MovementComponent; }

	UFUNCTION(BlueprintCallable, Category="Projectile")
	float GetProjectileDamage();

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	FProjectileValues GetProjectileValues();

	UFUNCTION(BlueprintCallable, Category="Projectile")
	void SetProjectileValues(FProjectileValues projectileValues);

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void ChangeProjCollision();

	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};