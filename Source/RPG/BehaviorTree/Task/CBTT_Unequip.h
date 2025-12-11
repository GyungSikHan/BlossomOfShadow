#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTT_Unequip.generated.h"

UCLASS()
class RPG_API UCBTT_Unequip : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UCBTT_Unequip();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
