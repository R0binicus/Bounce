// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BounceProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS(config=Game)
class ABounceProjectile : public AActor
{
	GENERATED_BODY()

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

protected:

	// Maximum number of boucnes before the projectile is destroyed
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	int MaxBounces = 5;

	// Current bounce value, when it gets to zero, it gets destroyed
	UPROPERTY()
	int CurrentBounce = MaxBounces;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float ProjectileDamage = 10.0f;

public:
	ABounceProjectile();

	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns CollisionComp subobject **/
	USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	float GetProjectileDamage();

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SetProjectileValues(int projBounces, float projSpeed, float projDamage, float bounciness, float gravAmount, float lifeTime);
};
