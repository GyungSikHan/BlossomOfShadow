#include "CBTT_MoveToRange.h"
#include "RPG/Utilities/CLog.h"
#include "Characters/AI/CEnemy_AI.h"
#include "RPG/Characters/AI/CAIController.h"
#include "RPG/Components/CAIBehaviorComponent.h"

UCBTT_MoveToRange::UCBTT_MoveToRange()
{
	NodeName = "Move To Range";
	bNotifyTick = true;
}

EBTNodeResult::Type UCBTT_MoveToRange::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCAIBehaviorComponent* be = Cast<UCAIBehaviorComponent>(ai->GetComponentByClass(UCAIBehaviorComponent::StaticClass()));

	float distance = ai->GetDistanceTo(be->GetTarget());
	

	if (be->GetDistanceToAttackTarget() < distance)
	{
		controller->MoveToActor(be->GetTarget(), 5);
		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Succeeded;
}

void UCBTT_MoveToRange::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCAIBehaviorComponent* be = Cast<UCAIBehaviorComponent>(ai->GetComponentByClass(UCAIBehaviorComponent::StaticClass()));
	float distance = ai->GetDistanceTo(be->GetTarget());

	if(be->GetDistanceToAttackTarget() >= distance)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
	else
	{
		controller->MoveToActor(be->GetTarget(), 5);
	}
}
