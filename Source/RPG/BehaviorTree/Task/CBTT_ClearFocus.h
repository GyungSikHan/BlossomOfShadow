#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTT_ClearFocus.generated.h"

UCLASS()
class RPG_API UCBTT_ClearFocus : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UCBTT_ClearFocus();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
