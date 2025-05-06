// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetManager.h"
#include "EventDispatcher.h"

// Sets default values
ATargetManager::ATargetManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATargetManager::BeginPlay()
{
	Super::BeginPlay();
	GameTimer = 1.0f;
}

// Called every frame
void ATargetManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GameTimer += DeltaTime;

	if (GameTimer >= 10.0f)
	{
		UEventDispatcher::GetEventManagerSingleton()->Event_WaveWeights.Broadcast(1, 1);
		GameTimer = 0.0f;
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("GameTimer: %f"), GameTimer));
	}
}

