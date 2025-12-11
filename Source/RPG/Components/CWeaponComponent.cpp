#include "CWeaponComponent.h"
#include "Utilities/CLog.h"
#include "CStateComponent.h"
#include "GameFramework/Character.h"
#include "RPG/Weapon/CDoAction.h"
#include "RPG/Weapon/CEquipment.h"
#include "RPG/Weapon/CWeaponAsset.h"
#include "RPG/Weapon/CWeaponData.h"
#include "RPG/Weapon/Skills/CSkills.h"

UCWeaponComponent::UCWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());

	for (int32 i = 0; i < (int32)EWeaponType::Max; i++)
		if (DataAssets[i] != nullptr)
			DataAssets[i]->BeginPlay(OwnerCharacter, &Datas[i]);

}

void UCWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (GetDoAction() != nullptr)
		GetDoAction()->Tick(DeltaTime);
	if (GetSkills() != nullptr)
		GetSkills()->Tick(DeltaTime);
}

bool UCWeaponComponent::IsIdleMode()
{
	return Cast<UCStateComponent>(OwnerCharacter->GetComponentByClass(UCStateComponent::StaticClass()))->IsIdleMode();
}

ACAttachment* UCWeaponComponent::GetAttachment()
{
	if (IsUnarmedMode() == true)
		return nullptr;

	if (Datas[(int32)Type] == nullptr)
		return nullptr;

	return Datas[(int32)Type]->GetAttachment();
}

UCEquipment* UCWeaponComponent::GetEquipment()
{
	if (IsUnarmedMode() == true)
		return nullptr;
	
	if (Datas[(int32)Type] == nullptr)
		return nullptr;

	return Datas[(int32)Type]->GetEquipment();
}

UCDoAction* UCWeaponComponent::GetDoAction()
{
	if (IsUnarmedMode() == true)
		return nullptr;
	if (Datas[(int32)Type] == nullptr)
		return nullptr;

	return Datas[(int32)Type]->GetDoAction();
}

UCSkills* UCWeaponComponent::GetSkills()
{
	if (IsUnarmedMode() == true)
		return nullptr;
	if (Datas[(int32)Type] == nullptr)
		return nullptr;

	return Datas[(int32)Type]->GetSkills((int32)SkillType);
}

void UCWeaponComponent::SetUnarmedMode()
{
	GetEquipment()->Unequip();

	ChangeType(EWeaponType::Max);
}

void UCWeaponComponent::SetSwordMode()
{
	if (IsIdleMode() == false)
		return;

	SetMode(EWeaponType::Sword);
}

void UCWeaponComponent::SetHammerMode()
{
	if(IsIdleMode() == false)
		return;

	SetMode(EWeaponType::Hammer);
}

void UCWeaponComponent::SetMagicRodMode()
{
	if (IsIdleMode() == false)
		return;

	SetMode(EWeaponType::MagicRod);
}

void UCWeaponComponent::SetBowMode()
{
	if (IsIdleMode() == false)
		return;

	SetMode(EWeaponType::Bow);
}

void UCWeaponComponent::SetWarpMode()
{
	if(IsIdleMode() == false)
		return;

	SetMode(EWeaponType::Warp);
}

void UCWeaponComponent::SetEquip(bool InIndex)
{
	bEquip = InIndex;
}

void UCWeaponComponent::DoAction()
{
	if (GetDoAction() != nullptr)
		GetDoAction()->DoAction();
}

void UCWeaponComponent::Skills_Pressed(ESkillsType InSkillType)
{
	SkillType = InSkillType;
	
	if (GetSkills() != nullptr)
		GetSkills()->Skill_Pressed();
}

void UCWeaponComponent::Skills_Released()
{
	if (GetSkills() != nullptr)
		GetSkills()->Skill_Released();
}

void UCWeaponComponent::SetMode(EWeaponType InType)
{
	if (Type == InType)
	{
		SetUnarmedMode();
		return;
	}
	else if (IsUnarmedMode() == false)
	{
		GetEquipment()->Unequip();
	}

	if(Datas[(int32)InType] != nullptr)
	{
		bEquip = true;
		if(Datas[(int32)InType] == nullptr)
			return;
		Data = Datas[(int32)InType];
		Datas[(int32)InType]->GetEquipment()->Equip();
		ChangeType(InType);
	}
}

void UCWeaponComponent::ChangeType(EWeaponType InType)
{
	EWeaponType prevType = Type;
	Type = InType;

	if (OnWeaponTypeChanged.IsBound())
		OnWeaponTypeChanged.Broadcast(prevType,Type);
}

void UCWeaponComponent::SetHitEquip()
{
	Data->GetEquipment()->HitEquip();
}