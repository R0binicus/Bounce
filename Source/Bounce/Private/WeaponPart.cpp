// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponPart.h"
#include "Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UWeaponPart::UWeaponPart()
{
	
}

bool UWeaponPart::EquipPart(UWeaponComponent* TargetWeapon)
{
	if(TargetWeapon == nullptr || TargetWeapon->GetAttachChildren().Contains(this)) false;
    Weapon = TargetWeapon;

	// FDetachmentTransformRules DetachmentRules(EDetachmentRule::KeepRelative, true);
	// DetachFromComponent(DetachmentRules);

	// Attach the weapon to the Player Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Weapon, AttachmentRules, FName(PartName));
	SetRelativeLocation(PartOffset);
	SetVisibility(true);

	return true;
}

void UWeaponPart::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// Maintain the EndPlay call chain
	Super::EndPlay(EndPlayReason);
}

FRotator UWeaponPart::RandDouble(float max, float min)
{
	double pitch = static_cast<double>(FMath::FRandRange(min, max));
	double yaw = static_cast<double>(FMath::FRandRange(min, max));
	double roll = static_cast<double>(FMath::FRandRange(min, max));

	return FRotator(pitch, yaw, roll);
}

void UWeaponPart::RandomizeValues()
{
	Scatter = FMath::FRandRange(0.0f, 10.0f);
	Amount = (int)FMath::RandRange(1, 10);
	FireRate = FMath::FRandRange(0.0f, 3.0f);
	RecoilAmount = FMath::FRandRange(0.0f, 7.0f);
	Damage = FMath::FRandRange(1.0f, 50.0f);
	Bounces = (int)FMath::RandRange(1, 10);
	Speed = FMath::FRandRange(100.0f, 10000.0f);
	Bounciness = FMath::FRandRange(0.1f, 3.0f);
	GravityEnabled = FMath::RandBool();
    GravityAmount = (GravityEnabled) ? FMath::FRandRange(0.0f, 5.0f) : 0;
	Lifespan = FMath::FRandRange(0.5f, 25.0f);
}