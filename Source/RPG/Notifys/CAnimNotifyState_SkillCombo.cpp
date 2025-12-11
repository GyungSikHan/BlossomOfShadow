// Fill out your copyright notice in the Description page of Project Settings.


#include "CAnimNotifyState_SkillCombo.h"

#include "RPG/Components/CWeaponComponent.h"
#include "RPG/Weapon/Skills/CSkills.h"
#include "RPG/Weapon/Skills/CSkills_AirCombo.h"
#include "Utilities/CLog.h"

FString UCAnimNotifyState_SkillCombo::GetNotifyName_Implementation() const
{
	return "Skill Combo";
}

void UCAnimNotifyState_SkillCombo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp == nullptr)
		return;
	if (MeshComp->GetOwner() == nullptr)
		return;

	UCWeaponComponent* weapon = Cast<UCWeaponComponent>(MeshComp->GetOwner()->GetComponentByClass(UCWeaponComponent::StaticClass()));

	if (weapon == nullptr)
		return;
	if (weapon->GetSkills() == nullptr)
		return;

	UCSkills_AirCombo* aircombo = Cast<UCSkills_AirCombo>(weapon->GetSkills());
	if(aircombo == nullptr)
		return;
	aircombo->EnableCombo();
}

void UCAnimNotifyState_SkillCombo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{

	if (MeshComp == nullptr)
		return;
	if (MeshComp->GetOwner() == nullptr)
		return;

	UCWeaponComponent* weapon = Cast<UCWeaponComponent>(MeshComp->GetOwner()->GetComponentByClass(UCWeaponComponent::StaticClass()));

	if (weapon == nullptr)
		return;
	if (weapon->GetSkills() == nullptr)
		return;

	UCSkills_AirCombo* aircombo = Cast<UCSkills_AirCombo>(weapon->GetSkills());
	if (aircombo == nullptr)
		return;
	
	aircombo->DisableCombo();
}
