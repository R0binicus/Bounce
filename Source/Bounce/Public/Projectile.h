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

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	UStaticMeshComponent* CollisionComponent;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Movement, meta=(AllowPrivateAccess="true"))
	UProjectileMovementComponent* MovementComponent;

public:
	AProjectile();

	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns CollisionComp subobject **/
	UStaticMeshComponent* GetCollisionComp() const { return CollisionComponent; }
	/** Returns ProjectileMovement subobject **/
	UProjectileMovementComponent* GetProjectileMovement() const { return MovementComponent; }

	UFUNCTION(BlueprintCallable, Category="Projectile")
	float GetProjectileDamage();

	UFUNCTION(BlueprintCallable, Category="Projectile")
	void SetProjectileValues(float _damage, int _bounces, float _speed, float _bounciness, float _gravity, float _lifespan, FVector _scale);

	/** Sound to play each time we bounce */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	USoundBase* BounceSound;

protected:
	// Damage dealt to hit actor on collision start
	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	float Damage = 10.0f;

	// Maximum number of bounces before the projectile is destroyed
	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	int Bounces = 5;

	// Maximum number of bounces before the projectile is destroyed
	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	FVector Scale = FVector(1.f, 1.f, 1.f);
};
