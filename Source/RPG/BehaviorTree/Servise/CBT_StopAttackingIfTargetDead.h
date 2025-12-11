#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBT_StopAttackingIfTargetDead.generated.h"

UCLASS()
class RPG_API UCBT_StopAttackingIfTargetDead : public UBTService
{
	GENERATED_BODY()
public:
	UCBT_StopAttackingIfTargetDead();
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
