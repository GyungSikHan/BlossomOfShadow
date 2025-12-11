#include "CEQS_Context_AttackTarget.h"
#include "Characters/AI/CEnemy_AI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "RPG/Utilities/CLog.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "RPG/Characters/AI/CAIController.h"
#include "RPG/Components/CAIBehaviorComponent.h"

UCEQS_Context_AttackTarget::UCEQS_Context_AttackTarget()
{
}

void UCEQS_Context_AttackTarget::ProvideContext(FEnvQueryInstance& QueryInstance,
                                                FEnvQueryContextData& ContextData) const
{
	if(Actor != nullptr)
	{
		TArray<AActor*> playerStart;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), playerStart);
		if(playerStart.Num() <= 0)
			return;
		APlayerStart* first = Cast<APlayerStart>(playerStart[0]);
		UEnvQueryItemType_Actor::SetContextHelper(ContextData,first);
		return;
	}

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(QueryInstance.Owner.Get());
	ACAIController* controller = ai->GetController<ACAIController>();
	UBlackboardComponent* blackboard = controller->GetBlackboardComponent();
	AActor* target = Cast<AActor>(blackboard->GetValueAsObject("Target"));

	UEnvQueryItemType_Actor::SetContextHelper(ContextData, target);
}
