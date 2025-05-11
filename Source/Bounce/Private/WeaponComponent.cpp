// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponent.h"
#include "PlayerCharacter.h"
#include "WeaponPart.h"
#include "Projectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Animation/AnimInstance.h"
#include "EventDispatcher.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
	PrimaryComponentTick.bCanEverTick = true;
}

void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(CanShoot) return;

	FireTimer -= DeltaTime;
	if(FireTimer <= 0.0f) CanShoot = true;
}

void UWeaponComponent::Fire()
{
	if(!CanShoot) return;
	if(Character == nullptr || Character->GetController() == nullptr) return;
    if(ProjectileClass == nullptr) return;
	if(FireSound == nullptr) return;
	if(FireAnimation == nullptr) return;

    UWorld* const World = GetWorld();
    if(World == nullptr) return;

	UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
	if(AnimInstance == nullptr) return;

    APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
    const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
    
    // MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
	const FVector OwnerLocation = GetOwner()->GetActorLocation();
    const FVector SpawnLocation = OwnerLocation+SpawnRotation.RotateVector(MuzzleOffset);
	const FVector KnockbackLocation = OwnerLocation+SpawnRotation.RotateVector(FVector(100.f, 0.f, 0.f));

    // Set Spawn Collision Handling Override
    FActorSpawnParameters ActorSpawnParams;
    ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

    // Spawn the specified amount of projectiles at the muzzle
    for(uint8 i = 0; i < Amount; ++i)
    {
        AProjectile* shot = World->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation+RandDouble(-Scatter, Scatter), ActorSpawnParams);
		if(shot == nullptr) continue;
		
		float momentum = Character->GetVelocity().GetMax()+Speed;
        shot->SetProjectileValues(Damage, Bounces, momentum, Bounciness, GravityAmount, Lifespan);
    }

    CanShoot = false;
    FireTimer = FireRate;

    // PlayerController->AddPitchInput(-RecoilAmount);
	Character->GetMovementComponent()->AddRadialImpulse(KnockbackLocation, 1000.f, KnockbackForce, ERadialImpulseFalloff::RIF_Linear, true);
    RandomizeValues();
	UEventDispatcher::GetEventManagerSingleton()->Event_RefireTime.Broadcast(FireTimer);
	
	// Play the firing sound
	UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	
	// Play the firing animation
	AnimInstance->Montage_Play(FireAnimation, 1.f);
}

bool UWeaponComponent::EquipWeapon(APlayerCharacter* TargetCharacter)
{
	if(TargetCharacter == nullptr || TargetCharacter->GetInstanceComponents().FindItemByClass<UWeaponComponent>()) return false;
    Character = TargetCharacter;

	FDetachmentTransformRules DetachmentRules(EDetachmentRule::KeepRelative, true);
	DetachFromComponent(DetachmentRules);

	// Attach the weapon to the Player Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));
	Rename(TEXT("Weapon"), Character);
	SetRelativeLocation(WeaponOffset);
	SetVisibility(true);

	// Set up action bindings
	if(APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			Subsystem->AddMappingContext(FireMappingContext, 1);

		// Set fire mapping
		if(UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UWeaponComponent::Fire);
	}

	CanShoot = true;
	return true;
}

bool UWeaponComponent::AttachPart(UWeaponPart* TargetPart)
{
	if(TargetPart == nullptr || WeaponParts.Contains(TargetPart)) return false;

	WeaponParts.Add(TargetPart);
	CalculateValues();
	return true;
}

void UWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if(Character == nullptr) return;
	
	// Remove the input mapping context from the Player Controller
	if(APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			Subsystem->RemoveMappingContext(FireMappingContext);
	}

	// Maintain the EndPlay call chain
	Super::EndPlay(EndPlayReason);
}

FRotator UWeaponComponent::RandDouble(float max, float min)
{
	double pitch = static_cast<double>(FMath::FRandRange(min, max));
	double yaw = static_cast<double>(FMath::FRandRange(min, max));
	double roll = static_cast<double>(FMath::FRandRange(min, max));

	return FRotator(pitch, yaw, roll);
}

void UWeaponComponent::ResetValues()
{
	Scatter = 0.f;
	Amount = 1;
	FireRate = 1.f;
	RecoilAmount = 0.f;
	Damage = 1.f;
	Bounces = 5;
	Speed = 15000.f;
	Bounciness = 1.f;
	GravityEnabled = true;
    GravityAmount = 1.f;
	Lifespan = 50.f;
}

void UWeaponComponent::RandomizeValues()
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

void UWeaponComponent::CalculateValues()
{
	ResetValues();

	for(int i = 0; i < WeaponParts.Num(); i++) {
		if(WeaponParts[i] == nullptr) {
			WeaponParts.RemoveAt(i);
			i--;
			continue;
		}

		Scatter += WeaponParts[i]->Scatter;
		Amount += WeaponParts[i]->Amount;
		FireRate += WeaponParts[i]->FireRate;
		RecoilAmount += WeaponParts[i]->RecoilAmount;
		Damage += WeaponParts[i]->Damage;
		Bounces += WeaponParts[i]->Bounces;
		Speed += WeaponParts[i]->Speed;
		Bounciness += WeaponParts[i]->Bounciness;
    	GravityAmount += WeaponParts[i]->GravityAmount;
		Lifespan += WeaponParts[i]->Lifespan;
	}
}