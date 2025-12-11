#include "CBT_UpdateDistanceTarget.h"

#include "RPG/Characters/AI/CAIController.h"
#include "RPG/Components/CAIBehaviorComponent.h"
#include "Kismet/KismetMathLibrary.h"

UCBT_UpdateDistanceTarget::UCBT_UpdateDistanceTarget()
{
	NodeName = "UpdateDistanceTarget";
	bNotifyTick = true;
}

void UCBT_UpdateDistanceTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCAIBehaviorComponent* behavior = Cast<UCAIBehaviorComponent>(ai->GetComponentByClass(UCAIBehaviorComponent::StaticClass()));

	ACharacter* target = behavior->GetTarget();
	if(target == nullptr)
		return;

	behavior->SetDistanceToAttackTarget(AttackRainge);
}
