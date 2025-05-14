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

private:
	/** The WeaponComponent this part is attached to */
	UWeaponComponent* Weapon;

public:
	/** Part offset location relative to owning weapon */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Part, meta=(AllowPrivateAccess="true"))
	FVector PartOffset;

	/** Identifiable name of the part */
	UPROPERTY(EditDefaultsOnly, Category="Part", meta=(AllowPrivateAccess="true"))
	FString PartName = "UnknownWeaponPart";
	
	/** These values are all ADDED to the base WeaponComponent stats */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float Scatter = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	int Amount = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float FireRate = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float RecoilAmount = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float KnockbackForce = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float Damage = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	int Bounces = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float Speed = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float Bounciness = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float GravityAmount = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float Lifespan = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	FVector Scale = FVector(0.f, 0.f, 0.f);

public:
	/** Sets default values for this component's properties */
	UWeaponPart();

protected:
	/** Ends gameplay for this component */
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void RandomizeValues();

	FRotator RandDouble(float max, float min);

public:
	/** Attaches the actor to a FirstPersonCharacter */
	UFUNCTION(BlueprintCallable, Category = "Part")
	bool EquipPart(UWeaponComponent* TargetWeapon);

	/** Converts all values into drawable text */
	UFUNCTION(BlueprintCallable, Category = "Part")
	FString DisplayStats();
};
