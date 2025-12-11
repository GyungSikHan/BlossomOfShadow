// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Weapon/Skills/CSkills.h"
#include "CSkills_Defence.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class RPG_API UCSkills_Defence : public UCSkills
{
	GENERATED_BODY()
private:
	UPROPERTY(EditDefaultsOnly, Category = "Debug")
	bool bDebug = false;
	UPROPERTY(EditDefaultsOnly, Category = "Radius")
	float Radius = 50.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Life")
	float LifeTime = 1.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	UAnimMontage* Montage;
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FHitData HitData;

public:
	FORCEINLINE bool GetParry() { return bParry; }

public:
	virtual void Skill_Pressed() override;

	virtual void Begin_Skill_Implementation() override;
	virtual void End_Skill_Implementation() override;
	virtual void Skill_Released() override;

	
	void CrateCollision();
	void DestroyCollision();

	void ParryAttack();
	UFUNCTION()
	void ResetTime();

	void SetParry(bool InIndex);
private:
	UFUNCTION()
	void  OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	ACharacter* Hitted;
	bool bParry;
	bool bDefence;
	float MontagePosition;
	class USphereComponent* Sphere;

	FTimerHandle Timer;
};

