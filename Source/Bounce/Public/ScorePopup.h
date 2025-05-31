// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TextRenderComponent.h"
#include "PlayerCharacter.h"
#include "GameFramework/Actor.h"
#include "ScorePopup.generated.h"

UCLASS()
class BOUNCE_API AScorePopup : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ScoreText")
	UTextRenderComponent* MyText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ScoreText")
	APlayerCharacter* Player;
	
public:


public:	
	// Sets default values for this actor's properties
	AScorePopup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void MakeTextFacePlayer();

public:
	
};
