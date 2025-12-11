// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CRPGMode.generated.h"

UCLASS(minimalapi)
class ACRPGMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACRPGMode();
protected:
	virtual void BeginPlay() override;
};



