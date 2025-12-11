#include "CBT_StopAttackingIfTargetDead.h"
#include "Characters/AI/CEnemy_AI.h"
#include "RPG/Characters/CPlayer.h"
#include "RPG/Characters/AI/CAIController.h"
#include "RPG/Components/CAIBehaviorComponent.h"

UCBT_StopAttackingIfTargetDead::UCBT_StopAttackingIfTargetDead()
{
	NodeName = "Stop Attacking If Target Dead";
}

void UCBT_StopAttackingIfTargetDead::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCAIBehaviorComponent* behavior = Cast<UCAIBehaviorComponent>(ai->GetComponentByClass(UCAIBehaviorComponent::StaticClass()));

	ACEnemy_AI* enemy = Cast<ACEnemy_AI>(behavior->GetTarget());
	ACPlayer* player = Cast<ACPlayer>(behavior->GetTarget());
	UCStateComponent* state{};
	if (enemy != nullptr)
		state = Cast<UCStateComponent>(enemy->GetComponentByClass(UCStateComponent::StaticClass()));
	else if (player != nullptr)
		state = Cast<UCStateComponent>(player->GetComponentByClass(UCStateComponent::StaticClass()));

	if(state != nullptr && state->IsDeadMode() != false)
	{
		controller->ClearFocus(EAIFocusPriority::Gameplay);
		controller->NotPoint();
		behavior->SetPassiveMode();
		return;
	}

}