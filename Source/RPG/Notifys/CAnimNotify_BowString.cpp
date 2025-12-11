#include "CAnimNotify_BowString.h"
#include "RPG/Components/CWeaponComponent.h"
#include "RPG/Weapon/DoActions/CDoAction_Bow.h"

FString UCAnimNotify_BowString::GetNotifyName_Implementation() const
{
	return "End_BowString";
}

void UCAnimNotify_BowString::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if(MeshComp == nullptr)
		return;
	if(MeshComp->GetOwner() == nullptr)
		return;

	UCWeaponComponent* weapon = Cast<UCWeaponComponent>(MeshComp->GetOwner()->GetComponentByClass(UCWeaponComponent::StaticClass()));
	if(weapon == nullptr)
		return;
	if(weapon->GetDoAction() == nullptr)
		return;

	UCDoAction_Bow* bow = Cast<UCDoAction_Bow>(weapon->GetDoAction());
	if(bow == nullptr)
		return;

	bow->End_BowString();
}

