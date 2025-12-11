#include "CBTT_AIAttack.h"

#include "RPG/Characters/AI/CAIController.h"
#include "RPG/Components/CAIBehaviorComponent.h"
#include "RPG/Components/CWeaponComponent.h"
#include "RPG/Weapon/CDoAction.h"

UCBTT_AIAttack::UCBTT_AIAttack()
{
	NodeName = "Attack";
	bNotifyTick = true;
}

EBTNodeResult::Type UCBTT_AIAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCAIBehaviorComponent* behavior = Cast<UCAIBehaviorComponent>(ai->GetComponentByClass(UCAIBehaviorComponent::StaticClass()));
	UCWeaponComponent* weapon = Cast<UCWeaponComponent>(ai->GetComponentByClass(UCWeaponComponent::StaticClass()));

	if (weapon == nullptr)
		return EBTNodeResult::Failed;

	controller->StopMovement();

	weapon->DoAction();
	return EBTNodeResult::InProgress;
}

void UCBTT_AIAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCAIBehaviorComponent* behavior = Cast<UCAIBehaviorComponent>(ai->GetComponentByClass(UCAIBehaviorComponent::StaticClass()));
	UCWeaponComponent* weapon = Cast<UCWeaponComponent>(ai->GetComponentByClass(UCWeaponComponent::StaticClass()));
	UCStateComponent* state = Cast<UCStateComponent>(ai->GetComponentByClass(UCStateComponent::StaticClass()));

	if(state == nullptr || weapon == nullptr)
		return;

	bool bCheck = true;
	bCheck &= state->IsIdleMode();
	bCheck &= weapon->GetDoAction()->GetInAction() == false;

	if (bCheck == true)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		controller->StopMovement();
		return;
	}
}

EBTNodeResult::Type UCBTT_AIAttack::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	if (ai == nullptr)
		return EBTNodeResult::Failed;
	UCAIBehaviorComponent* behavior = Cast<UCAIBehaviorComponent>(ai->GetComponentByClass(UCAIBehaviorComponent::StaticClass()));
	UCWeaponComponent* weapon = Cast<UCWeaponComponent>(ai->GetComponentByClass(UCWeaponComponent::StaticClass()));
	UCStateComponent* state = Cast<UCStateComponent>(ai->GetComponentByClass(UCStateComponent::StaticClass()));

	if (state == nullptr || weapon == nullptr || behavior == nullptr)
		return EBTNodeResult::Failed;

	if (state->IsHittdMode() == true )
		if(weapon->GetDoAction()->GetInAction() == true)
			weapon->GetDoAction()->End_DoAction();
	
	return EBTNodeResult::Aborted;
}
