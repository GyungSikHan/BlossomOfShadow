#include "AnimNotify_EndBowSkill.h"
#include "RPG/Components/CMovementComponent.h"
#include "RPG/Components/CStateComponent.h"
#include "RPG/Components/CWeaponComponent.h"
#include "GameFramework/Character.h"
#include "RPG/Weapon/CDoAction.h"

FString UAnimNotify_EndBowSkill::GetNotifyName_Implementation() const
{
	return "EndBowSkill";
}

void UAnimNotify_EndBowSkill::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp == nullptr)
		return;
	ACharacter* character = Cast<ACharacter>(MeshComp->GetOwner());
	if (character == nullptr)
		return;
	UCStateComponent* State = Cast<UCStateComponent>(character->GetComponentByClass(UCStateComponent::StaticClass()));
	if (State == nullptr)
		return;
	UCMovementComponent* Movement = Cast<UCMovementComponent>(character->GetComponentByClass(UCMovementComponent::StaticClass()));
	if (Movement == nullptr)
		return;

	Movement->Move();
	Movement->EnableControlRotation();
	Movement->DisableFixedCamera();

	State->SetIdleMode();
	State->OffSkillMode();

	UCWeaponComponent* Weapon = Cast<UCWeaponComponent>(character->GetComponentByClass(UCWeaponComponent::StaticClass()));
	if (Weapon == nullptr)
		return;
	if (Weapon->IsBowMode() == false)
		return;
	//Weapon->GetSkillMode() = false;

	Weapon->GetDoAction()->End_DoAction();
}
