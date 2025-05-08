// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "WeaponComponent.generated.h"

class APlayerCharacter;

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BOUNCE_API UWeaponComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	/** Weapon location offset */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Weapon, meta=(AllowPrivateAccess="true"))
	FVector WeaponOffset;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Weapon)
	FVector MuzzleOffset;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Weapon)
	USoundBase* FireSound;
	
	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Weapon)
	UAnimMontage* FireAnimation;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess="true"))
	class UInputMappingContext* FireMappingContext;

	/** Fire Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess="true"))
	class UInputAction* FireAction;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class AProjectile> ProjectileClass;

	/** Sets default values for this component's properties */
	UWeaponComponent();

	/** Attaches the actor to a FirstPersonCharacter */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	bool EquipWeapon(APlayerCharacter* TargetCharacter);

	/** Make the weapon Fire a Projectile */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void Fire();

protected:
	/** Ends gameplay for this component */
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void RandomizeValues();
	
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float Scatter = 4.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	int Amount = 2;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float FireRate = 0.5f;

	UPROPERTY()
	bool CanShoot = false;

	UPROPERTY()
	float FireTimer;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float RecoilAmount = 0.5f;

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
	/** The Character holding this weapon */
	APlayerCharacter* Character;

	FRotator RandDouble(float max, float min);
};
