// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CSkills.h"
#include "CSkills_Warp.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class RPG_API UCSkills_Warp : public UCSkills
{
	GENERATED_BODY()
public:
	UCSkills_Warp();

	virtual void BeginPlay(class ACharacter* InOwner, class ACAttachment* InAttachment, class UCDoAction* InDoAction) override;
	virtual void Tick_Implementation(float InDeltaTime) override;

public:
	virtual void Skill_Pressed();
	virtual void Skill_Released();

private:
	bool GetCursorLocationAndRotation(FVector& OutLocation, FRotator& OutRotation);

private:
	class APlayerController* PlayerController;
	class UDecalComponent* Decal;
	class UCAIBehaviorComponent* Behavior;

private:
	FVector MoveToLocation;

};
