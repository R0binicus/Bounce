// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CheckpointArea.generated.h"

class UBoxComponent;
class USceneComponent;

UCLASS()
class BOUNCE_API ACheckpointArea : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = Checkpoint)
	UBoxComponent* BoxComp;

	/** Scene component root*/
	UPROPERTY(VisibleDefaultsOnly, Category = Checkpoint)
	USceneComponent* SceneCompRoot;

	/** Scene component, used for the respawn position*/
	UPROPERTY(VisibleDefaultsOnly, Category = Checkpoint)
	USceneComponent* SceneCompRespawn;

	UPROPERTY(BlueprintReadOnly, Category = Checkpoint)
	FVector RespawnPosition;
public:	
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Sets default values for this actor's properties
	ACheckpointArea();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
