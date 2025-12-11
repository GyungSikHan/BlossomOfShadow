#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTT_Hitted.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UCBTT_Hitted : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UCBTT_Hitted();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
