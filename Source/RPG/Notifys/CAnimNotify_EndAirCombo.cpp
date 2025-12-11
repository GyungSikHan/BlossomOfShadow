// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifys/CAnimNotify_EndAirCombo.h"

#include "Components/CMovementComponent.h"
#include "Components/CStateComponent.h"
#include "Utilities/CLog.h"
#include "Components/CWeaponComponent.h"
#include "GameFramework/Character.h"
#include "Weapon/Skills/CSkills.h"

FString UCAnimNotify_EndAirCombo::GetNotifyName_Implementation() const
{
	return "End Air Combo";
}

void UCAnimNotify_EndAirCombo::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (MeshComp == nullptr)
		return;
	if (MeshComp->GetOwner() == nullptr)
		return;
	UCMovementComponent* Movement = Cast<UCMovementComponent>(MeshComp->GetOwner()->GetComponentByClass(UCMovementComponent::StaticClass()));
	if (Movement == nullptr)
		return;
	UCStateComponent* state = Cast<UCStateComponent>(MeshComp->GetOwner()->GetComponentByClass(UCStateComponent::StaticClass()));
	if (state == nullptr)
		return;
	UCWeaponComponent* weapon = Cast<UCWeaponComponent>(MeshComp->GetOwner()->GetComponentByClass(UCWeaponComponent::StaticClass()));
	if(weapon == nullptr)
		return;
	if(weapon->GetSkills() == nullptr)
		return;
	state->SetIdleMode();
	state->OffSkillMode();
	Movement->Move();
	Movement->EnableControlRotation();
	Movement->DisableFixedCamera();
	weapon->GetSkills()->End_Skill_Implementation();
}
