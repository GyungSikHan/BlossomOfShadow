#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CWeaponStructures.h"
#include "CEquipment.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipmentBeginEquip);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipmentUnequip);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipmentEquip);

UCLASS(Blueprintable)
class RPG_API UCEquipment : public UObject
{
	GENERATED_BODY()
public:
	FORCEINLINE bool GetBeginEquip() { return bBeginEquip; }
	FORCEINLINE const bool* GetEquipped() { return &bEquipped; }

public:
	void BeginPlay(ACharacter* InOwner, const FEquipmentData& InData);

public:
	UFUNCTION(BlueprintNativeEvent)
		void Equip();
	virtual void Equip_Implementation();
	UFUNCTION(BlueprintNativeEvent)
		void Begin_Equip();
	virtual void Begin_Equip_Implementation();
	UFUNCTION(BlueprintNativeEvent)
		void End_Equip();
	virtual void End_Equip_Implementation();
	UFUNCTION(BlueprintNativeEvent)
		void Unequip();
	virtual void Unequip_Implementation();

public:
	FEquipmentBeginEquip OnEquipmentBeginEquip;
	FEquipmentUnequip OnEquipmentUnequip;
	FEquipmentEquip OnEquipmentEquip;

public:
	void HitEquip();
public:
	UPROPERTY(BlueprintReadOnly)
		class ACharacter* OwnerCharacter;

private:
	FEquipmentData Data;

private:
	class UCWeaponComponent* Weapon;
	class UCMovementComponent* Movement;
	class UCStateComponent* State;

private:
	bool bBeginEquip;
	bool bEquipped;

};
