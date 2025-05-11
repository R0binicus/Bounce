// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Use a sphere as a simple collision representation
	CollisionComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	CollisionComponent->SetRelativeScale3D(FVector(Scale));
	CollisionComponent->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComponent->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

	// Players can't walk on it
	CollisionComponent->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComponent->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComponent;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->UpdatedComponent = CollisionComponent;
	MovementComponent->InitialSpeed = 3000.f;
	MovementComponent->MaxSpeed = 3000.f;
	MovementComponent->bRotationFollowsVelocity = true;
	MovementComponent->bShouldBounce = true;

	// Die after 10 seconds by default
	InitialLifeSpan = 10.0f;
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (BounceSound == nullptr) return;
	Bounces--;
	if (Bounces <= 0)
	{
		Destroy();
		return;
	}

	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		if (OtherComp->GetCollisionProfileName() == "Target")
		{
			Destroy();
			return;
		}
	}

	// Play the firing sound
	UGameplayStatics::PlaySoundAtLocation(this, BounceSound, GetActorLocation());
}

float AProjectile::GetProjectileDamage()
{
	return Damage;
}

void AProjectile::SetProjectileValues(float _damage, int _bounces, float _speed, float _bounciness, float _gravity, float _lifespan, FVector _scale)
{
	Damage = _damage;
	Bounces = _bounces;
	Scale = _scale;

	MovementComponent->InitialSpeed = _speed;
	MovementComponent->MaxSpeed = _speed;
	MovementComponent->Bounciness = _bounciness;
	MovementComponent->ProjectileGravityScale = _gravity;
	SetLifeSpan(_lifespan);
	CollisionComponent->SetRelativeScale3D(Scale);
}