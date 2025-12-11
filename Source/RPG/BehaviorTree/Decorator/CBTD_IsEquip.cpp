// Fill out your copyright notice in the Description page of Project Settings.


#include "CBTD_IsEquip.h"

#include "RPG/Characters/AI/CAIController.h"
#include "RPG/Components/CWeaponComponent.h"
#include "Characters/AI/CEnemy_AI.h"
UCBTD_IsEquip::UCBTD_IsEquip()
{
	NodeName = "IsEquip";
}

bool UCBTD_IsEquip::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCWeaponComponent* weapon = Cast<UCWeaponComponent>(ai->GetComponentByClass(UCWeaponComponent::StaticClass()));
	if (weapon == nullptr)
		return false;
	return weapon->IsUnarmedMode();
}