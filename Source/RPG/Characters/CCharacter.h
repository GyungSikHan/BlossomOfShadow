// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "GameFramework/Character.h"
#include "Interface/ICharacter.h"
#include "IMPStamina.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CCharacter.generated.h"

UCLASS()
class RPG_API ACCharacter : public ACharacter, public IICharacter, public IIMPStamina
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Team")
	uint8 TeamID = 1;

protected:
	//Component
	UPROPERTY(VisibleAnywhere)
	class UCGameInstance* GameInstance;
	
	UPROPERTY(VisibleAnywhere)
	class UCMovementComponent* Move;
	UPROPERTY(VisibleAnywhere)
	class UCStateComponent* State;
	UPROPERTY(VisibleAnywhere)
	class UCWeaponComponent* Weapon;
	UPROPERTY(VisibleAnywhere)
	class UCStatusComponent* Status;
	UPROPERTY(VisibleAnywhere)
	class UCMontagesComponent* Montage;
	UPROPERTY(VisibleAnywhere)
	class UCTargetComponent* Target;

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* SceneAirAttack;
	UPROPERTY(VisibleAnywhere)
	class UArrowComponent* AirCollision;

protected:
	//UI
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UUserWidget> LabelWidgetClass;

	
private:
	//Debug
	UPROPERTY(EditAnywhere, Category = "Trace")
	TEnumAsByte<EDrawDebugTrace::Type> DebugType;

public:
	FORCEINLINE USceneComponent* GetSceneComponent() { return SceneAirAttack; }
	FORCEINLINE UArrowComponent* GetAirArrowComponent() { return AirCollision; }
	FORCEINLINE FVector GetPrvAirCollisionLocation() { return PrvAirCollisionLocation; }
	FORCEINLINE FGenericTeamId GetGenericTeamId() const { return FGenericTeamId(TeamID); }
	FORCEINLINE uint8 GetTeamID() { return TeamID; }
	FORCEINLINE	float GetTeam() { return TeamID; }
	FORCEINLINE float GetHP() { return Status->GetHealth(); }
	FORCEINLINE UCWeaponComponent* GetWeapon() { return Weapon; }

public:
	ACCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void Landed(const FHitResult& Hit) override;

protected:
	UFUNCTION()
	virtual void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);

protected:
	virtual void Hitted();
	virtual void Dead();
	virtual void End_Hitted() override;
	virtual void End_Dead() override;

	float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;


	void AirBuffer(float AirBuff);

public:
	void SetInterruptible(bool InValue);

protected:
	struct FDamageData
	{
		float Power;
		class ACCharacter* Character;
		class AActor* Causer;

		struct FActionDamageEvent* Event;
	} Damage;

protected:
	FVector PrvAirCollisionLocation;
	FRotator CapsuleRotation;
	bool bInterruptible;
	bool bAirComboDamage;
};
