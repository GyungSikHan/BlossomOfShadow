#include "CBTT_Unequip.h"

#include "RPG/Characters/AI/CAIController.h"
#include "RPG/Components/CWeaponComponent.h"

UCBTT_Unequip::UCBTT_Unequip()
{
	NodeName = "Unequip";
}

EBTNodeResult::Type UCBTT_Unequip::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCWeaponComponent* weapon = Cast<UCWeaponComponent>(ai->GetComponentByClass(UCWeaponComponent::StaticClass()));

	if (weapon->IsUnarmedMode() == true)
		return EBTNodeResult::Failed;

	weapon->SetUnarmedMode();
	return EBTNodeResult::Succeeded;
}
