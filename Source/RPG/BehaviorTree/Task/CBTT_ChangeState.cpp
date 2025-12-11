#include "CBTT_ChangeState.h"
#include "Characters/AI/CEnemy_AI.h"
#include "RPG/Characters/AI/CAIController.h"
UCBTT_ChangeState::UCBTT_ChangeState()
{
	NodeName = "ChangeType";
}

EBTNodeResult::Type UCBTT_ChangeState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCAIBehaviorComponent* behavior = Cast<UCAIBehaviorComponent>(ai->GetComponentByClass(UCAIBehaviorComponent::StaticClass()));

	switch (Type)
	{
	case EAIStateType::Passive:
		behavior->SetPassiveMode();
		controller->NotPoint();
		break;
	case EAIStateType::Attacking:
		behavior->SetAttackMode();
		break;
	case EAIStateType::Hitted:
		behavior->SetHittedMode();
		break;
	case EAIStateType::Investigating:
		behavior->SetInvestigateMode();
		break;
	case EAIStateType::Seeking:
		behavior->SetSeekingMode();
		break;
	case EAIStateType::Approach:
		behavior->SetApproachMode();
		break;
	case EAIStateType::Dead:
		behavior->SetDeadMode();
		break;
	default:
		break;
	}

	return EBTNodeResult::Succeeded;
}
