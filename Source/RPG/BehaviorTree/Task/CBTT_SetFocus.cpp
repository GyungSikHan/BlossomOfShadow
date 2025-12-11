#include "CBTT_SetFocus.h"
#include "RPG/Utilities/CLog.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "RPG/Characters/AI/CAIController.h"
#include "RPG/Components/CAIBehaviorComponent.h"

UCBTT_SetFocus::UCBTT_SetFocus()
{
	NodeName = "SetFocus";
}

EBTNodeResult::Type UCBTT_SetFocus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCAIBehaviorComponent* behavior = Cast<UCAIBehaviorComponent>(ai->GetComponentByClass(UCAIBehaviorComponent::StaticClass()));
	controller->SetFocus(behavior->GetTarget());
	return EBTNodeResult::Succeeded;
}
