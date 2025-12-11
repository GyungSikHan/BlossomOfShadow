#include "CBTT_Patrol.h"

#include "NavigationSystem.h"
#include "RPG/BehaviorTree/CPatrolPath.h"
#include "RPG/Characters/AI/CAIController.h"
#include "RPG/Components/CAIBehaviorComponent.h"
#include "Navigation/PathFollowingComponent.h"

UCBTT_Patrol::UCBTT_Patrol()
{
	NodeName = "Patrol";
	bNotifyTick = true;
}

EBTNodeResult::Type UCBTT_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCAIBehaviorComponent* behavior = Cast<UCAIBehaviorComponent>(ai->GetComponentByClass(UCAIBehaviorComponent::StaticClass()));

	if (ai->GetPatrolPath() != nullptr)
	{

		FVector moveToPoint = ai->GetPatrolPath()->GetMoveTo();
		behavior->SetPatrolLocation(moveToPoint);

		if (bDebugMode == true)
			DrawDebugSphere(ai->GetWorld(), moveToPoint, 25, 25, FColor::Green, true, 5);

		return EBTNodeResult::InProgress;
	}

	FVector location = ai->GetActorLocation();
	UNavigationSystemV1* navSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(ai->GetWorld());

	if (navSystem == nullptr)
		return EBTNodeResult::Failed;

	FNavLocation point(location);
	bool b{};
	while (b == false)
	{
		if (navSystem->GetRandomPointInNavigableRadius(location, RandomRadius, point) == true)
			b = true;
	}

	behavior->SetPatrolLocation(point.Location);

	if (bDebugMode == true)
		DrawDebugSphere(ai->GetWorld(), point.Location, 25, 25, FColor::Green, true, 5);

	return EBTNodeResult::InProgress;
}

void UCBTT_Patrol::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCAIBehaviorComponent* behavior = Cast<UCAIBehaviorComponent>(ai->GetComponentByClass(UCAIBehaviorComponent::StaticClass()));

	FVector location = behavior->GetPatrolLocation();
	EPathFollowingRequestResult::Type result = controller->MoveToLocation(location, AcceptanceDistance, false);

	switch (result)
	{
	case EPathFollowingRequestResult::Type::Failed:
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		break;

	case EPathFollowingRequestResult::Type::AlreadyAtGoal:
		if (ai->GetPatrolPath())
			ai->GetPatrolPath()->UpdateIndex();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		break;

	default:
		break;
	}
}