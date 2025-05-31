// Fill out your copyright notice in the Description page of Project Settings.

#include "ScorePopup.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"

// Sets default values
AScorePopup::AScorePopup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MyText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("RotatingText"));
	MyText->SetWorldSize(70.f);
	MyText->SetTextRenderColor(FColor::Red);
	MyText->SetHorizontalAlignment(EHTA_Center);
}

// Called when the game starts or when spawned
void AScorePopup::BeginPlay()
{
	Super::BeginPlay();
	Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
}

// Called every frame
void AScorePopup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MakeTextFacePlayer();
}

void AScorePopup::MakeTextFacePlayer()
{
	FRotator NewRotation = Player->GetActorRotation();
	NewRotation.Yaw += 180.f;

	MyText->SetRelativeRotation(NewRotation);
}