#include "CAnimNotifyState_Combo.h"
#include "RPG/Components/CWeaponComponent.h"
#include "RPG/Weapon/DoActions/CDoAction_Combo.h"
#include "RPG/Weapon/DoActions/CDoAction_Magic.h"

FString UCAnimNotifyState_Combo::GetNotifyName_Implementation() const
{
	return "Combo";
}

void UCAnimNotifyState_Combo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (MeshComp == nullptr)
		return;
	if(MeshComp->GetOwner() == nullptr)
		return;

	UCWeaponComponent* weapon = Cast<UCWeaponComponent>(MeshComp->GetOwner()->GetComponentByClass(UCWeaponComponent::StaticClass()));

	if(weapon == nullptr)
		return;
	if(weapon->GetDoAction() == nullptr)
		return;

	if (weapon->IsMagicRodMode() == true)
	{
		UCDoAction_Magic* com = Cast<UCDoAction_Magic>(weapon->GetDoAction());
		if(com == nullptr)
			return;
		com->EnableCombo();
		return;
	}
	UCDoAction_Combo* combo = Cast<UCDoAction_Combo>(weapon->GetDoAction());
	if (combo == nullptr)
		return;

	combo->EnableCombo();
}

void UCAnimNotifyState_Combo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
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
	if (weapon->GetDoAction() == nullptr)
		return;

	if (weapon->IsMagicRodMode() == true)
	{
		UCDoAction_Magic* com = Cast<UCDoAction_Magic>(weapon->GetDoAction());
		if (com == nullptr)
			return;
		com->DisableCombo();
		return;
	}

	UCDoAction_Combo* combo = Cast<UCDoAction_Combo>(weapon->GetDoAction());
	if (combo == nullptr)
		return;

	combo->DisableCombo();
}



