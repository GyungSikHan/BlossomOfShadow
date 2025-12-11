// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifys/CAnimNotifyState_Parry.h"

#include "Characters/CCharacter.h"
#include "Components/CWeaponComponent.h"
#include "Utilities/CLog.h"
#include "Weapon/Skills/CSkills_Defence.h"

FString UCAnimNotifyState_Parry::GetNotifyName_Implementation() const
{
	return "Parry";
}

void UCAnimNotifyState_Parry::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	if (MeshComp == nullptr)
		return;
	if (MeshComp->GetOwner() == nullptr)
		return;

	UCWeaponComponent* weapon = Cast<UCWeaponComponent>(MeshComp->GetOwner()->GetComponentByClass(UCWeaponComponent::StaticClass()));
	if(weapon == nullptr)
		return;
	
	UCSkills_Defence* defence = Cast<UCSkills_Defence>(weapon->GetSkills());
	if(defence == nullptr)
		return;
	defence->SetParry(true);
}

void UCAnimNotifyState_Parry::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (MeshComp == nullptr)
		return;
	if (MeshComp->GetOwner() == nullptr)
		return;

	UCWeaponComponent* weapon = Cast<UCWeaponComponent>(MeshComp->GetOwner()->GetComponentByClass(UCWeaponComponent::StaticClass()));
	if (weapon == nullptr)
		return;

	UCSkills_Defence* defence = Cast<UCSkills_Defence>(weapon->GetSkills());
	if (defence == nullptr)
		return;
	defence->SetParry(false);

}
