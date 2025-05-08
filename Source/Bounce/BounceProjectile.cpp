// Copyright Epic Games, Inc. All Rights Reserved.

#include "BounceProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

ABounceProjectile::ABounceProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(1.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &ABounceProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 10 seconds by default
	InitialLifeSpan = 10.0f;
}

void ABounceProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	/*if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
	}*/

	CurrentBounce--;
	if (CurrentBounce <= 0)
	{
		Destroy();
	}
}

float ABounceProjectile::GetProjectileDamage()
{
	return ProjectileDamage;
}

void ABounceProjectile::SetProjectileValues(int projBounces, float projSpeed, float projDamage, float bounciness, float gravAmount, float lifeTime)
{
	MaxBounces = projBounces;
	CurrentBounce = projBounces;
	ProjectileMovement->InitialSpeed = projSpeed;
	ProjectileMovement->MaxSpeed = projSpeed;
	ProjectileDamage = projDamage;
	ProjectileMovement->Bounciness = bounciness;
	ProjectileMovement->ProjectileGravityScale = gravAmount;
	SetLifeSpan(lifeTime);
}