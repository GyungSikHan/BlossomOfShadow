#include "CAnimNotifyState_Collistion.h"
#include "RPG/Components/CWeaponComponent.h"
#include "RPG/Weapon/CAttachment.h"

FString UCAnimNotifyState_Collistion::GetNotifyName_Implementation() const
{
	return "Collision";
}

void UCAnimNotifyState_Collistion::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (MeshComp == nullptr)
		return;
	if (MeshComp->GetOwner() == nullptr)
		return;

	UCWeaponComponent* weapon = Cast<UCWeaponComponent>(MeshComp->GetOwner()->GetComponentByClass(UCWeaponComponent::StaticClass()));

	if (weapon  == nullptr)
		return;
	if(weapon->GetAttachment() == nullptr)
		return;

	weapon->GetAttachment()->OnCollisions();

}

void UCAnimNotifyState_Collistion::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
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
	if (weapon->GetAttachment() == nullptr)
		return;

	weapon->GetAttachment()->OffCollisions();
}
