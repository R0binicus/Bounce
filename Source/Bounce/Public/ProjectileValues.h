// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileValues.generated.h"

USTRUCT(BlueprintType)
struct FProjectileValues
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileValues")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileValues")
	int Bounces;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileValues")
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileValues")
	float Bounciness;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileValues")
	float Gravity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileValues")
	float Lifetime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileValues")
	FVector Scale;
};
