#include "CBTT_SetSpeed.h"

#include "RPG/Characters/AI/CAIController.h"

UCBTT_SetSpeed::UCBTT_SetSpeed()
{
	NodeName = "Speed";
}

EBTNodeResult::Type UCBTT_SetSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCMovementComponent* move = Cast<UCMovementComponent>(ai->GetComponentByClass(UCMovementComponent::StaticClass()));

	move->SetSpeed(Type);
	return EBTNodeResult::Succeeded;
}
