// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponPart.h"
#include "Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UWeaponPart::UWeaponPart()
{
	PrimaryComponentTick.bCanEverTick = true;
	//RandomizeValues();
	SetVisibility(false, false);
}

bool UWeaponPart::EquipPart(UWeaponComponent* TargetWeapon)
{
	if(TargetWeapon == nullptr) return false;

	Weapon = TargetWeapon;
	if(Weapon->AttachPart(this)) SetVisibility(true, false);

	FDetachmentTransformRules DetachmentRules(EDetachmentRule::KeepRelative, true);
	DetachFromComponent(DetachmentRules);

	// Attach the weapon to the Player Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Weapon, AttachmentRules, FName(PartName));
	Rename(*PartName, Weapon);
	//SetRelativeLocation(PartOffset);

	return true;
}

FString UWeaponPart::DisplayStats() {
	//FString text = FString::Printf(TEXT("Weapon Part {\n  Scatter += &f\n  Bullets += &d\n  Fire Rate += &f\n  Recoil += &f\n  Damage += &f\n  Bounces += &d\n  Speed += &f\n  Rebound += &f\n  Gravity += &f\n  Lifespan += &f\n"), Scatter, Amount, FireRate, RecoilAmount, Damage, Bounces, Speed, Bounciness, GravityAmount, Lifespan);
	return(PartName);
}

void UWeaponPart::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// Maintain the EndPlay call chain
	Super::EndPlay(EndPlayReason);
}

void UWeaponPart::RandomizeValues()
{
	Scatter = FMath::FRandRange(0.0f, 1.0f);
	Amount = (int)FMath::RandRange(1, 2);
	FireRate = FMath::FRandRange(-0.01f, -0.05f);
	RecoilAmount = FMath::FRandRange(0.0f, 1.0f);
	Damage = FMath::FRandRange(0.1f, 1.0f);
	Bounces = (int)FMath::RandRange(-1, 5);
	Speed = FMath::FRandRange(-100.0f, 300.0f);
	Bounciness = FMath::FRandRange(0.01f, 0.03f);
    GravityAmount = FMath::FRandRange(-0.3f, 0.03f);
	Lifespan = FMath::FRandRange(0.1f, 3.0f);
	float s = FMath::FRandRange(0.01f, 0.03f);
	Scale = FVector(s);
}

FRotator UWeaponPart::RandDouble(float max, float min)
{
	double pitch = static_cast<double>(FMath::FRandRange(min, max));
	double yaw = static_cast<double>(FMath::FRandRange(min, max));
	double roll = static_cast<double>(FMath::FRandRange(min, max));

	return FRotator(pitch, yaw, roll);
}