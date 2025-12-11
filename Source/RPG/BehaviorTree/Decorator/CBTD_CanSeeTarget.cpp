#include "CBTD_CanSeeTarget.h"

#include "RPG/Characters/AI/CAIController.h"
#include "RPG/Components/CAIBehaviorComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UCBTD_CanSeeTarget::UCBTD_CanSeeTarget()
{
	NodeName = "Can See Target";
}

bool UCBTD_CanSeeTarget::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCAIBehaviorComponent* behavior = Cast<UCAIBehaviorComponent>(ai->GetComponentByClass(UCAIBehaviorComponent::StaticClass()));
	ACharacter* target = behavior->GetTarget();

	if (target == nullptr)
		return false;

	TArray<AActor*> ignore;
	ignore.AddUnique(ai);
	FHitResult hit;
	bool trace = UKismetSystemLibrary::LineTraceSingle(ai->GetWorld(), ai->GetActorLocation(), target->GetActorLocation(), ETraceTypeQuery::TraceTypeQuery1, false, ignore, Debug, hit, true);

	return !trace;
}
