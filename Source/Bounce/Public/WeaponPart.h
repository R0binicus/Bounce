// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "WeaponComponent.h"
#include "WeaponPart.generated.h"

class UWeaponComponent;

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BOUNCE_API UWeaponPart : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	/** Part offset location relative to owning weapon */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Part, meta=(AllowPrivateAccess="true"))
	FVector PartOffset;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class AProjectile> ProjectileClass;

	/** Sets default values for this component's properties */
	UWeaponPart();

	/** Attaches the actor to a FirstPersonCharacter */
	UFUNCTION(BlueprintCallable, Category="Part")
	bool EquipPart(UWeaponComponent* TargetWeapon);

protected:
	/** Ends gameplay for this component */
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void RandomizeValues();

	/** Identifiable name of the part */
	UPROPERTY(EditDefaultsOnly, Category="Part", meta=(AllowPrivateAccess="true"))
	FString PartName = "UnknownWeaponPart";
	
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float Scatter = 4.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	int Amount = 2;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float FireRate = 0.5f;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float RecoilAmount = 0.5f;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float KnockbackForce = 1000.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float Damage = 10.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	int Bounces = 5;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float Speed = 3000.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float Bounciness = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	bool GravityEnabled = false;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float GravityAmount = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float Lifespan = 10.0f;

private:
	/** The WeaponComponent this part is attached to */
	UWeaponComponent* Weapon;

	FRotator RandDouble(float max, float min);
};
