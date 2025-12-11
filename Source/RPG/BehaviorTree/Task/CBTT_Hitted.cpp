// Fill out your copyright notice in the Description page of Project Settings.


#include "CBTT_Hitted.h"
#include "Characters/AI/CEnemy_AI.h"
#include "RPG/Characters/AI/CAIController.h"
UCBTT_Hitted::UCBTT_Hitted()
{
	bNotifyTick = true;
	NodeName = "Hitted";
}

EBTNodeResult::Type UCBTT_Hitted::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCStateComponent* state = Cast<UCStateComponent>(ai->GetComponentByClass(UCStateComponent::StaticClass()));

	if (state == nullptr)
		return EBTNodeResult::Failed;

	controller->StopMovement();
	controller->ClearFocus(EAIFocusPriority::Gameplay);
	controller->StopMovement();
	return EBTNodeResult::Succeeded;
}

void UCBTT_Hitted::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	//ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	//ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	//UCStateComponent* state = Cast<UCStateComponent>(ai->GetComponentByClass(UCStateComponent::StaticClass()));

	//if(state->IsHittdMode() == true || state->IsIdleMode() == true)
	//{
	//	state->SetIdleMode();
	//	controller->StopMovement();
	//	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	//	return;
	//}
	
}
