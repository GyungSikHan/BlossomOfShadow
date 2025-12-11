#include "CAnimNotify_EndSkill.h"
#include "RPG/Components/CMovementComponent.h"
#include "RPG/Components/CStateComponent.h"
#include "GameFramework/Character.h"
#include "RPG/Components/CWeaponComponent.h"
#include "RPG/Weapon/Skills/CSkills.h"

FString UCAnimNotify_EndSkill::GetNotifyName_Implementation() const
{
	return "EndSkill";
}

void UCAnimNotify_EndSkill::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp == nullptr)
		return;
	ACharacter* character = Cast<ACharacter>(MeshComp->GetOwner());
	if(character == nullptr)
		return;
	UCStateComponent* State = Cast<UCStateComponent>(character->GetComponentByClass(UCStateComponent::StaticClass()));
	if(State == nullptr)
		return;
	UCMovementComponent* Movement = Cast<UCMovementComponent>(character->GetComponentByClass(UCMovementComponent::StaticClass()));
	if(Movement == nullptr)
		return;
	UCWeaponComponent* Weapon = Cast<UCWeaponComponent>(character->GetComponentByClass(UCWeaponComponent::StaticClass()));
	if(Weapon == nullptr)
		return;

	State->SetIdleMode();
	State->OffSkillMode();
	State->SetInterruptible(false);
	State->SetInterruptible(false);

	Movement->Move();
	Movement->EnableControlRotation();
	Movement->DisableFixedCamera();
	Weapon->GetSkills()->Skill_Released();
}
