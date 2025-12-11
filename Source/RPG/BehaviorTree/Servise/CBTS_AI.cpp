#include "CBTS_AI.h"

#include "RPG/Characters/CPlayer.h"
#include "RPG/Utilities/CLog.h"
#include "RPG/Characters/AI/CAIController.h"
#include "RPG/Components/CAIBehaviorComponent.h"

UCBTS_AI::UCBTS_AI()
{
	NodeName = "Basic";
	bNotifyTick = true;
}

void UCBTS_AI::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCStateComponent* state = Cast<UCStateComponent>(ai->GetComponentByClass(UCStateComponent::StaticClass()));
	UCAIBehaviorComponent* behavior = Cast<UCAIBehaviorComponent>(ai->GetComponentByClass(UCAIBehaviorComponent::StaticClass()));

	if (bDrawDebug)
	{
		FVector start = ai->GetActorLocation();
		start.Z -= 25;

		FVector end = start;

		DrawDebugCylinder(ai->GetWorld(), start, end, ActionRange, 10, FColor::Red, false, Interval);
	}
	if (state->IsDeadMode() == true)
	{
		controller->ClearFocus(EAIFocusPriority::Gameplay);
		return;
		
	}
	if(state->IsHittdMode() == true)
	{

		behavior->SetHittedMode();
		return;
	}

	if (state->IsHittdMode() == true)
		return;

	ACharacter* target = behavior->GetTarget();

	if(target == nullptr)
	{
		if(controller->GetPoint() == true)
		{
			behavior->SetInvestigateMode();
			return;
		}
		controller->ClearFocus(EAIFocusPriority::Gameplay);
		behavior->SetPassiveMode();
		return;
	}

	if(behavior->GetTeamID() == ai->GetTeamID())
		return;

	controller->SetFocus(target);

	float distnace = ai->GetDistanceTo(target);
	if (distnace <= ActionRange)
	{
		behavior->SetAttackMode();
		return;
	}


	behavior->SetApproachMode();
}
