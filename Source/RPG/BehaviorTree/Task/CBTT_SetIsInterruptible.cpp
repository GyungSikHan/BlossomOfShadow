#include "CBTT_SetIsInterruptible.h"

#include "RPG/Characters/AI/CAIController.h"

UCBTT_SetIsInterruptible::UCBTT_SetIsInterruptible()
{
	NodeName = "Set Interruptible";
}

EBTNodeResult::Type UCBTT_SetIsInterruptible::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	if (ai == nullptr)
		return EBTNodeResult::Failed;

	ai->SetInterruptible(bValue);
	return EBTNodeResult::Succeeded;
}
