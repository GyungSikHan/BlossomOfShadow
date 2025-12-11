#include "CAnimNotifyState_Skills.h"

#include "Components/CStateComponent.h"
#include "RPG/Components/CMovementComponent.h"
#include "RPG/Components/CWeaponComponent.h"
#include "RPG/Weapon/Skills/CSkills.h"

FString UCAnimNotifyState_Skills::GetNotifyName_Implementation() const
{
	return "Skills";
}

void UCAnimNotifyState_Skills::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if(MeshComp == nullptr)
		return;
	if(MeshComp->GetOwner() == nullptr)
		return;

	UCWeaponComponent* weapon = Cast<UCWeaponComponent>(MeshComp->GetOwner()->GetComponentByClass(UCWeaponComponent::StaticClass()));
	if(weapon == nullptr)
		return;
	if(weapon->GetSkills() == nullptr)
		return;

	weapon->GetSkills()->Begin_Skill_Implementation();
}

void UCAnimNotifyState_Skills::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
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
	if (weapon->GetSkills() == nullptr)
		return;

	weapon->GetSkills()->End_Skill_Implementation();
}
