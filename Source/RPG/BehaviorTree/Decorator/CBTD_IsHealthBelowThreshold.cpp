#include "CBTD_IsHealthBelowThreshold.h"

#include "Components/CStatusComponent.h"
#include "RPG/Characters/AI/CAIController.h"
#include "Characters/AI/CEnemy_AI.h"
UCBTD_IsHealthBelowThreshold::UCBTD_IsHealthBelowThreshold()
{
	NodeName = "Is Health Below Threshold";
}

bool UCBTD_IsHealthBelowThreshold::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCStatusComponent* status = Cast<UCStatusComponent>(ai->GetComponentByClass(UCStatusComponent::StaticClass()));

	float currHp = status->GetHealth();
	float maxHp = status->GetMaxHealth();

	return (currHp / maxHp) <= HealthThreshold;
}
