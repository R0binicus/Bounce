// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileValues.h"
#include "Components/SkeletalMeshComponent.h"
#include "WeaponComponent.generated.h"

class APlayerCharacter;
class UWeaponPart;

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BOUNCE_API UWeaponComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

private:
	/** The Character holding this weapon */
	APlayerCharacter* Character;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Defaults")
	float DefaultScatter = 50.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Defaults")
	int DefaultAmount = 10;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Defaults")
	float DefaultFireRate = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Defaults")
	float DefaultRecoilAmount = 0.5f;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Defaults")
	float DefaultKnockbackForce = 1000.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Defaults")
	FProjectileValues DefaultProjectileValues = FProjectileValues(1.f, 10, 15000.f, 1.f, 0.f, 50.f, FVector(1.f, 1.f, 1.f));

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float Scatter = DefaultScatter;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	int Amount = DefaultAmount;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float FireRate = DefaultFireRate;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float RecoilAmount = DefaultRecoilAmount;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float KnockbackForce = DefaultKnockbackForce;

	UPROPERTY(BlueprintReadOnly, Category = "Projectile")
	FProjectileValues ProjectileValues = DefaultProjectileValues;

	UPROPERTY()
	bool CanShoot = false;

	UPROPERTY()
	float FireTimer;

public:
	/** Weapon location offset */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	FVector WeaponOffset;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	FVector MuzzleOffset;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	UAnimMontage* FireAnimation;

	/** Weapon parts attached */
	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	TArray<class UWeaponPart*> WeaponParts;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* FireMappingContext;

	/** Fire Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* FireAction;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AProjectile> ProjectileClass;

public:
	UWeaponComponent();

protected:
	/** Ends gameplay for this component */
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void ResetValues();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void CalculateValues();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void RandomizeValues();

	FRotator RandDouble(float max, float min);

public:
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Make the weapon Fire a Projectile */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Fire();

	/** Attaches the actor to a FirstPersonCharacter */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool EquipWeapon(APlayerCharacter* TargetCharacter);

	/** Add a new weapon part to the array */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool AttachPart(UWeaponPart* TargetPart);
};
