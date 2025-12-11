// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/Character.h"
#include "UObject/NoExportTypes.h"
#include "CWeaponStructures.generated.h"

USTRUCT()
struct FEquipmentData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		class UAnimMontage* Montage{};
	UPROPERTY(EditAnywhere)
		float PlayRate = 1.0f;
	UPROPERTY(EditAnywhere)
		bool bCanMove = true;
	UPROPERTY(EditAnywhere)
		bool bUseControlRotation = true;
};

USTRUCT()
struct FDoActionData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		class UAnimMontage* Montage{};
	UPROPERTY(EditAnywhere)
		float PlayRate = 1.0f;
	UPROPERTY(EditAnywhere)
		float MP = 0.0f;
	UPROPERTY(EditAnywhere)
		float Stamina = 0.0f;
	UPROPERTY(EditAnywhere)
		bool bCanMove = true;
	UPROPERTY(EditAnywhere)
		bool bFixedCamera{};
	//UPROPERTY(EditAnywhere)
	//	class USoundWave* Sound;
	UPROPERTY(EditAnywhere)
		class UFXSystemAsset* Effect{};
	UPROPERTY(EditAnywhere)
		FVector EffectLocation = FVector::ZeroVector;
	UPROPERTY(EditAnywhere)
		FVector EffectScale = FVector::ZeroVector;
	UPROPERTY(EditAnywhere)
	bool bInterruptible{};

public:
	void DoAction(ACharacter* InOwner);
	void StopAction(ACharacter* InOwner);
	//void PlaySound(ACharacter* InOwner);
	void PlayEffect(UWorld* InWorld, const FVector& InLocation);
	void PlayEffect(UWorld* InWorld, const FVector& InLocation, const FRotator& InRotation);
};

USTRUCT()
struct FHitData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		class UAnimMontage* Montage{};
	UPROPERTY(EditAnywhere)
		float PlayRate = 1.0f;
	UPROPERTY(EditAnywhere)
		float Power{};
	UPROPERTY(EditAnywhere)
	float Launch = 100.0f;
	UPROPERTY(EditAnywhere)
		float StopTime{};
	UPROPERTY(EditAnywhere)
		class USoundWave* Sound{};
	UPROPERTY(EditAnywhere)
	UFXSystemAsset* Effect{};
	UPROPERTY(EditAnywhere)
		FVector EffectLocation = FVector::ZeroVector;
	UPROPERTY(EditAnywhere)
		FVector EffectScale = FVector::ZeroVector;
	UPROPERTY(EditAnywhere)
	bool IsFalling{};


public:
	void SendDamage(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther);
	void PlayMontage(ACharacter* InOwner);
	void PlayHitStop(UWorld* InWorld);
	void PlaySound(ACharacter* InOwner);
	void PlayEffect(UWorld* InWorld, const FVector& InLocation);
	void PlayEffect(UWorld* InWorld, const FVector& InLocation, const FRotator& InRotation);
};

USTRUCT()
struct FSubAction
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		UAnimMontage* Montage{};
	UPROPERTY(EditAnywhere)
		float PlayRate = 1.0f;
	UPROPERTY(EditAnywhere)
		bool bCanMove = true;
	UPROPERTY(EditAnywhere)
		bool bFixedCamera{};

public:
	void DoSubAction(ACharacter* InOwner);
};


USTRUCT()
struct FActionDamageEvent : public FDamageEvent
{
	GENERATED_BODY()

public:
	FHitData* HitData{};
};

UCLASS()
class RPG_API UCWeaponStructures : public UObject
{
	GENERATED_BODY()
	
};
