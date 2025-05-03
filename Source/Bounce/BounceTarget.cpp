// Fill out your copyright notice in the Description page of Project Settings.


#include "BounceTarget.h"
#include "DrawDebugHelpers.h"
#include "Components/BoxComponent.h"

// Sets default values
ABounceTarget::ABounceTarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MyCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	MyCollisionBox->SetCollisionProfileName("Target");

	RootComponent = MyCollisionBox;

	//MyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MY MESH"));
	//MyMesh->SetupAttachment(RootComponent);

	MyCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ABounceTarget::OnComponentBeginOverlap);
}

// Called when the game starts or when spawned
void ABounceTarget::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABounceTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//DrawDebugSphere(GetWorld(), GetActorLocaton(), SphereRadius, 20, FColor::Purple, false, -1, 0, 1);
}

void ABounceTarget::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
	}

	UE_LOG(LogClass, Log, TEXT("Target Hit"));

	Destroy();
}

void ABounceTarget::OnComponentBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		UE_LOG(LogClass, Log, TEXT("Target Overlap"));
		Destroy();
	}
}


