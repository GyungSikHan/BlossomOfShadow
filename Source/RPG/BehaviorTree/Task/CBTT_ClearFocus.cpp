#include "CBTT_ClearFocus.h"
#include "RPG/Characters/AI/CAIController.h"

UCBTT_ClearFocus::UCBTT_ClearFocus()
{
	NodeName = "Clear Focus";
}

EBTNodeResult::Type UCBTT_ClearFocus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());

	if (controller == nullptr)
		return EBTNodeResult::Failed;

	controller->ClearFocus(EAIFocusPriority::Gameplay);
	return EBTNodeResult::Succeeded;
}
