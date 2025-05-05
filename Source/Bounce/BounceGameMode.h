// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BounceGameMode.generated.h"

UCLASS(minimalapi)
class ABounceGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABounceGameMode();

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;
};