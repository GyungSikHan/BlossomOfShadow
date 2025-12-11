#include "CAnimNotify_EndAction.h"
#include "RPG/Components/CWeaponComponent.h"
#include "RPG/Components/CStateComponent.h"
#include "RPG/Weapon/CDoAction.h"

FString UCAnimNotify_EndAction::GetNotifyName_Implementation() const
{
	return "End_DoAction";
}

void UCAnimNotify_EndAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp == nullptr)
		return;
	if(MeshComp->GetOwner() == nullptr)
		return;

	UCWeaponComponent* weapon = Cast<UCWeaponComponent>(MeshComp->GetOwner()->GetComponentByClass(UCWeaponComponent::StaticClass()));

	if(weapon == nullptr)
		return;
	if(weapon->GetDoAction() == nullptr)
		return;
	UCStateComponent* state = Cast<UCStateComponent>(MeshComp->GetOwner()->GetComponentByClass(UCStateComponent::StaticClass()));
	if (state == nullptr)
		return;
	state->SetInterruptible(false);

	weapon->GetDoAction()->End_DoAction();
}
