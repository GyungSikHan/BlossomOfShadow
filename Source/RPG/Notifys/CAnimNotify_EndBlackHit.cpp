// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifys/CAnimNotify_EndBlackHit.h"

#include "Components/CWeaponComponent.h"
#include "Weapon/Skills/CSkills_Defence.h"

FString UCAnimNotify_EndBlackHit::GetNotifyName_Implementation() const
{
	return "Black End";
}

void UCAnimNotify_EndBlackHit::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if(MeshComp == nullptr)
		return;
	if(MeshComp->GetOwner() == nullptr)
		return;

	UCWeaponComponent* weapon = Cast<UCWeaponComponent>(MeshComp->GetOwner()->GetComponentByClass(UCWeaponComponent::StaticClass()));
	if(weapon == nullptr)
		return;

	UCSkills_Defence* defence = Cast<UCSkills_Defence>(weapon->GetSkills());
	if(defence == nullptr)
		return;
	defence->ResetTime();
}
