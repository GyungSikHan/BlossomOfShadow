// Fill out your copyright notice in the Description page of Project Settings.


#include "CAnimNotify_BeginAirCombo.h"

#include "RPG/Components/CWeaponComponent.h"
#include "RPG/Weapon/Skills/CSkills.h"

FString UCAnimNotify_BeginAirCombo::GetNotifyName_Implementation() const
{
	return "Begin AirCombo";
}

void UCAnimNotify_BeginAirCombo::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp == nullptr)
		return;
	if (MeshComp->GetOwner() == nullptr)
		return;

	UCWeaponComponent* weapon = Cast<UCWeaponComponent>(MeshComp->GetOwner()->GetComponentByClass(UCWeaponComponent::StaticClass()));

	if (weapon == nullptr)
		return;
	if (weapon->GetSkills() == nullptr)
		return;

	weapon->GetSkills()->Begin_Skill_Implementation();
}
