#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapon/CWeaponData.h"
#include "CWeaponComponent.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Sword=0, Hammer, MagicRod, Bow, Warp, Max
};

UENUM()
enum class ESkillsType
{
	Skill1, Skill2, Skill3, Skill4, Skill5, Max
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWeaponTypeChanged, EWeaponType, InPrevType, EWeaponType, InNewType);

UCLASS( ClassGroup = (Custom), meta = (BlueprintSpawnableComponent) )
class RPG_API UCWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "DataAsset")
		class UCWeaponAsset* DataAssets[(int32)EWeaponType::Max];

private:
	UPROPERTY()
		class UCWeaponData* Datas[(int32)EWeaponType::Max];

public:
	FORCEINLINE EWeaponType GetWeaponType() { return Type; }

	FORCEINLINE bool IsUnarmedMode() { return Type == EWeaponType::Max; }
	FORCEINLINE bool IsSwordMode() { return Type == EWeaponType::Sword; }
	FORCEINLINE bool IsHammerMode() { return Type == EWeaponType::Hammer; }
	FORCEINLINE bool IsMagicRodMode() { return Type == EWeaponType::MagicRod; }
	FORCEINLINE bool IsBowMode() { return Type == EWeaponType::Bow; }
	FORCEINLINE bool IsWarpMode() { return Type == EWeaponType::Warp; }
	FORCEINLINE bool GetEquip() { return bEquip; }

public:	
	UCWeaponComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	bool IsIdleMode();

public:
	class ACAttachment* GetAttachment();
	class UCEquipment* GetEquipment();
	class UCDoAction* GetDoAction();
public:
	UFUNCTION(BlueprintCallable)
	class UCSkills* GetSkills();

public:
	void SetUnarmedMode();
	void SetSwordMode();
	UFUNCTION(BlueprintCallable)
	void SetHammerMode();
	void SetMagicRodMode();
	void SetBowMode();
	void SetWarpMode();
	void SetEquip(bool InIndex);

	void DoAction();

public:
	UFUNCTION(BlueprintCallable)
	void Skills_Pressed(ESkillsType InSkillType);
	UFUNCTION(BlueprintCallable)
	void Skills_Released();

private:
	void SetMode(EWeaponType InType);
	void ChangeType(EWeaponType InType);

public:
	void SetHitEquip();

public:
	FWeaponTypeChanged OnWeaponTypeChanged;

private:
	class ACharacter* OwnerCharacter;
	EWeaponType Type = EWeaponType::Max;

private:
	ESkillsType SkillType;
	bool bEquip;

	UCWeaponData* Data;
};




