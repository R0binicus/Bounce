// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileValues.generated.h"

USTRUCT(BlueprintType)
struct FProjectileValues
{
	GENERATED_BODY()

	// Empty constructor
	FProjectileValues()
		: Damage(0.f)
		, Bounces(0)
		, Speed(0.f)
		, Bounciness(0.f)
		, Gravity(0.f)
		, Lifetime(0.f)
		, Scale(FVector::OneVector)
	{}

	// Prefilled constructor
	FProjectileValues(float damage, int bounces, float speed, float bounciness, float gravity, float lifetime, FVector scale)
		: Damage(damage)
		, Bounces(bounces)
		, Speed(speed)
		, Bounciness(bounciness)
		, Gravity(gravity)
		, Lifetime(lifetime)
		, Scale(scale)
	{}

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
