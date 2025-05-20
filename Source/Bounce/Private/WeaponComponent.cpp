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
#include "ProjectileValues.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
	PrimaryComponentTick.bCanEverTick = true;
}

void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	ResetValues();

	for(int i = 0; i < WeaponParts.Num(); i++) {
		if(WeaponParts[i] == nullptr) {
			WeaponParts.RemoveAt(i);
			i--;
			continue;
		}

		// Attach the weapon to the Player Character
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepRelative, true);
		WeaponParts[i]->AttachToComponent(this, AttachmentRules, FName(WeaponParts[i]->PartName));
		//SetRelativeLocation(PartOffset);
	}
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
	if(Character == nullptr) return;
	if(Character->GetController() == nullptr) return;
    if(ProjectileClass == nullptr) return;
	if(FireSound == nullptr) return;
	if(FireAnimation == nullptr) return;

    UWorld* const World = GetWorld();
    if(World == nullptr) return;

	UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
	if(AnimInstance == nullptr) return;

    APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
    const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
    
    // Establish spawn point for projectiles based on camera rotation
	const FVector SpawnOffset = SpawnRotation.RotateVector(FVector(150.f, 0.f, 0.f));
    const FVector SpawnLocation = GetOwner()->GetActorLocation()+SpawnOffset+FVector(0.f, 0.f, -10.f);
	const FVector KnockbackLocation = SpawnLocation+SpawnOffset;

    // Set Spawn Collision Handling Override
    FActorSpawnParameters ActorSpawnParams;
    ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

    // Spawn the specified amount of projectiles at the muzzle
	int shots = 0;
    for(uint8 i = 0; i < Amount; ++i) {
        AProjectile* shot = World->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation+RandDouble(-Scatter, Scatter), ActorSpawnParams);
		if(shot == nullptr) continue;
		
		float momentum = Character->GetVelocity().GetMax()+ ProjectileValues.Speed;
        shot->SetProjectileValues(FProjectileValues(ProjectileValues.Damage, ProjectileValues. Bounces, momentum, ProjectileValues.Bounciness, ProjectileValues.Gravity, ProjectileValues.Lifetime, ProjectileValues.Scale));
		shots++;
    }
	if(shots == 0) return;

    CanShoot = false;
    FireTimer = FireRate;

    // PlayerController->AddPitchInput(-RecoilAmount);
	Character->GetMovementComponent()->AddRadialImpulse(KnockbackLocation, 1000.f, KnockbackForce, ERadialImpulseFalloff::RIF_Linear, true);
    //RandomizeValues();
	UEventDispatcher::GetEventManagerSingleton()->Event_RefireTime.Broadcast(FireTimer);
	
	// Play the firing sound
	UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	
	// Play the firing animation
	AnimInstance->Montage_Play(FireAnimation, 1.f);
}

bool UWeaponComponent::EquipWeapon(APlayerCharacter* TargetCharacter)
{
	if(TargetCharacter == nullptr) return false;
	if(TargetCharacter->GetInstanceComponents().FindItemByClass<UWeaponComponent>()) return false;
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
	if(APlayerController* PlayerController = Cast<APlayerController>(Character->GetController())) {
		// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			Subsystem->AddMappingContext(FireMappingContext, 1);

		// Set fire mapping
		if(UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UWeaponComponent::Fire);
	}

	Character->SetWeaponComponent(this);
	CanShoot = true;
	return true;
}

bool UWeaponComponent::AttachPart(UWeaponPart* TargetPart)
{
	bool show = true;

	for(int i = 0; i < WeaponParts.Num(); i++) {
		if(WeaponParts[i] == nullptr) continue;
		if(WeaponParts[i]->PartName == TargetPart->PartName) {
			//Item already equipped so don't render it again
			show = false;
			break;
		}
	}

	WeaponParts.Add(TargetPart);
	CalculateValues();

	return show;
}

void UWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if(Character == nullptr) return;
	
	// Remove the input mapping context from the Player Controller
	if(APlayerController* PlayerController = Cast<APlayerController>(Character->GetController())) {
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			Subsystem->RemoveMappingContext(FireMappingContext);
	}

	// Maintain the EndPlay call chain
	Super::EndPlay(EndPlayReason);
}

void UWeaponComponent::ResetValues()
{
	Scatter = DefaultScatter;
	Amount = DefaultAmount;
	FireRate = DefaultFireRate;
	RecoilAmount = DefaultRecoilAmount;
	ProjectileValues.Damage = DefaultProjectileValues.Damage;
	ProjectileValues.Bounces = DefaultProjectileValues.Bounces;
	ProjectileValues.Speed = DefaultProjectileValues.Speed;
	ProjectileValues.Bounciness = DefaultProjectileValues.Bounciness;
	ProjectileValues.Gravity = DefaultProjectileValues.Gravity;
	ProjectileValues.Lifetime = DefaultProjectileValues.Lifetime;
	ProjectileValues.Scale = DefaultProjectileValues.Scale;
}

void UWeaponComponent::CalculateValues()
{
	ResetValues();

	for(int i = 0; i < WeaponParts.Num(); i++) {
		if(WeaponParts[i] == nullptr) continue;

		Scatter += WeaponParts[i]->Scatter;
		Amount += WeaponParts[i]->Amount;
		FireRate += WeaponParts[i]->FireRate;
		RecoilAmount += WeaponParts[i]->RecoilAmount;
		ProjectileValues.Damage += WeaponParts[i]->Damage;
		//ProjectileValues.Bounces += WeaponParts[i]->Bounces;
		ProjectileValues.Speed += WeaponParts[i]->Speed;
		//ProjectileValues.Bounciness += WeaponParts[i]->Bounciness;
		//ProjectileValues.Gravity += WeaponParts[i]->GravityAmount;
		//ProjectileValues.Lifetime += WeaponParts[i]->Lifespan;
		//ProjectileValues.Scale += WeaponParts[i]->Scale;
	}

	if (Amount < 1) Amount = 1;
	if (FireRate < 0.1f) FireRate = 0.1f;
	if (FireRate > 5.f) FireRate = 5.f;
	if (RecoilAmount < 0.f) RecoilAmount = 0.f;
	if (ProjectileValues.Damage < 0.1f) ProjectileValues.Damage = 0.1f;
	//if (ProjectileValues.Bounces < 0) ProjectileValues.Bounces = 0;
	if (ProjectileValues.Speed < 1000.f) ProjectileValues.Speed = 1000.f;
}

void UWeaponComponent::RandomizeValues()
{
	Scatter = FMath::FRandRange(0.0f, 10.0f);
	Amount = (int)FMath::RandRange(1, 10);
	FireRate = FMath::FRandRange(0.0f, 3.0f);
	RecoilAmount = FMath::FRandRange(0.0f, 7.0f);
	ProjectileValues.Damage = FMath::FRandRange(1.0f, 50.0f);
	ProjectileValues.Bounces = (int)FMath::RandRange(1, 10);
	ProjectileValues.Speed = FMath::FRandRange(100.0f, 10000.0f);
	ProjectileValues.Bounciness = FMath::FRandRange(0.1f, 3.0f);
	bool GravityEnabled = FMath::RandBool();
	ProjectileValues.Gravity = (GravityEnabled) ? FMath::FRandRange(0.0f, 5.0f) : 0;
	ProjectileValues.Lifetime = FMath::FRandRange(0.5f, 25.0f);
	float s = FMath::FRandRange(1.0f, 2.5f);
	ProjectileValues.Scale = FVector(s, s, s);
}

FRotator UWeaponComponent::RandDouble(float max, float min)
{
	double pitch = static_cast<double>(FMath::FRandRange(min, max));
	double yaw = static_cast<double>(FMath::FRandRange(min, max));
	double roll = static_cast<double>(FMath::FRandRange(min, max));

	return FRotator(pitch, yaw, roll);
}