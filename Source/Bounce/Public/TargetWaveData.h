// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TargetWaveData.generated.h"

USTRUCT(BlueprintType)
struct FTargetWaveData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TargetWaveData")
	int WaveThreshold = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TargetWaveData")
	TMap<TSubclassOf<class ABounceTarget>, int> WaveAmounts;
};
