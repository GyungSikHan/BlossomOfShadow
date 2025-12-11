// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CWarp.generated.h"

UCLASS(Blueprintable)
class RPG_API ACWarp : public AActor
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere)
	UDecalComponent* DecalComponent;

public:	
	ACWarp();

protected:
	virtual void BeginPlay() override;
};
