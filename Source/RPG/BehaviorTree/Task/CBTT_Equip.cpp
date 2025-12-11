#include "CBTT_Equip.h"
#include "RPG/Utilities/CLog.h"
#include "Characters/AI/CEnemy_AI.h"
#include "RPG/Characters/AI/CAIController.h"
#include "RPG/Components/CWeaponComponent.h"
#include "RPG/Weapon/CEquipment.h"

UCBTT_Equip::UCBTT_Equip()
{
	NodeName = "Equip";
	bNotifyTick = true;
}

EBTNodeResult::Type UCBTT_Equip::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCWeaponComponent* Weapon = Cast<UCWeaponComponent>(ai->GetComponentByClass(UCWeaponComponent::StaticClass()));

	if (Type == Weapon->GetWeaponType())
		return EBTNodeResult::Succeeded;

	switch (Type)
	{
	case EWeaponType::Sword:
		Weapon->SetSwordMode();
		break;
	case EWeaponType::Hammer:
		Weapon->SetHammerMode();
		break;
	case EWeaponType::MagicRod:
		Weapon->SetMagicRodMode();
		break;
	case EWeaponType::Bow:
		Weapon->SetBowMode();
		break;
	case EWeaponType::Warp:
		Weapon->SetWarpMode();
		break;
	default:
		break;
	}

	return  EBTNodeResult::InProgress;
}

void UCBTT_Equip::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	UCWeaponComponent* weapon = Cast<UCWeaponComponent>(ai->GetComponentByClass(UCWeaponComponent::StaticClass()));

	const bool* bEquipped = weapon->GetEquipment()->GetEquipped();
	
	UCStateComponent* state = Cast<UCStateComponent>(ai->GetComponentByClass(UCStateComponent::StaticClass()));

	if (state->IsIdleMode() )
	{
		if(bEquipped == nullptr)
		{
			CLog::Print(ai->GetName());
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			return;
		}
		else if(*bEquipped == true)
		{
			
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			controller->StopMovement();
			return;
		}
	
	}
}

EBTNodeResult::Type UCBTT_Equip::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	if (ai == nullptr)
		return EBTNodeResult::Failed;

	UCWeaponComponent* weapon = Cast<UCWeaponComponent>(ai->GetComponentByClass(UCWeaponComponent::StaticClass()));
	UCStateComponent* state = Cast<UCStateComponent>(ai->GetComponentByClass(UCStateComponent::StaticClass()));

	if (weapon == nullptr)
		return EBTNodeResult::Failed;
	bool bBeginEquip = weapon->GetEquipment()->GetBeginEquip();
	if (bBeginEquip == false)
		weapon->GetEquipment()->Begin_Equip();

	if (state->IsIdleMode() == true)
		return EBTNodeResult::Succeeded;
	weapon->GetEquipment()->End_Equip();

	return EBTNodeResult::Aborted;
}
