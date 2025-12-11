#include "CEquipment.h"
#include "Utilities/CLog.h"
#include "CWeaponStructures.h"
#include "Components/CWeaponComponent.h"
#include "GameFramework/Character.h"
#include "RPG/Components/CMovementComponent.h"
#include "RPG/Components/CStateComponent.h"

void UCEquipment::BeginPlay(ACharacter* InOwner, const FEquipmentData& InData)
{
	OwnerCharacter = InOwner;
	Data = InData;

	Weapon = Cast<UCWeaponComponent>(OwnerCharacter->GetComponentByClass(UCWeaponComponent::StaticClass()));
	Movement = Cast<UCMovementComponent>(OwnerCharacter->GetComponentByClass(UCMovementComponent::StaticClass()));
	State = Cast<UCStateComponent>(OwnerCharacter->GetComponentByClass(UCStateComponent::StaticClass()));
}

void UCEquipment::Equip_Implementation()
{
	State->SetEquipMode();

	if (OnEquipmentEquip.IsBound())
		OnEquipmentEquip.Broadcast();

	if (Data.bCanMove == false)
		Movement->Stop();

	if (Data.bUseControlRotation == true)
		Movement->EnableControlRotation();

	if(State->IsHittdMode() == true ||Data.Montage == nullptr)
	{
		//CLog::Print("C");
		Begin_Equip();
		End_Equip();
	}
	else if(Data.Montage != nullptr)
		OwnerCharacter->PlayAnimMontage(Data.Montage, Data.PlayRate);
}

void UCEquipment::Begin_Equip_Implementation()
{
	bBeginEquip = true;
	
	if (OnEquipmentBeginEquip.IsBound())
		OnEquipmentBeginEquip.Broadcast();
}

void UCEquipment::End_Equip_Implementation()
{
	bBeginEquip = false;
	bEquipped = true;

	Movement->Move();
	State->SetIdleMode();
	Weapon->SetEquip(false);
}

void UCEquipment::Unequip_Implementation()
{
	bBeginEquip = false;
	Movement->DisableControlRotation();

	if (OnEquipmentUnequip.IsBound())
		OnEquipmentUnequip.Broadcast();
}

void UCEquipment::HitEquip()
{
	if(State->IsHittdMode() == false && Weapon->GetEquip() == false)
		return;
	Begin_Equip_Implementation();
	End_Equip_Implementation();
}
