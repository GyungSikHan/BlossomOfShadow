// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CSkills.h"
#include "CSkills_AirCombo.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class RPG_API UCSkills_AirCombo : public UCSkills
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category = "Action")
	float HitSphereRadius = 100;
	UPROPERTY(EditAnywhere)
	float LifeTime = 0.1;
	UPROPERTY(EditAnywhere, Category = "Action")
	bool bDebug;
	UPROPERTY(EditAnywhere, Category = "Action")
	TArray<FHitData> HitDatas;


public:
	FORCEINLINE void EnableCombo() { bEnable = true; }
	FORCEINLINE void DisableCombo() { bEnable = false; }

public:
	UCSkills_AirCombo();
	virtual void Skill_Pressed() override;
	virtual void Begin_Skill_Implementation() override;
	virtual void End_Skill_Implementation() override;
	virtual void Tick_Implementation(float InDeltaTime) override;

private:
	void DrawCollision();
	void HitActor(TArray<FOverlapResult> Results);
	void Land();

private:
	bool bEnable;
	bool bExsit;
	TArray<ACharacter*> Hitted;
	int32 HitIndex;

	bool bTest;
};

