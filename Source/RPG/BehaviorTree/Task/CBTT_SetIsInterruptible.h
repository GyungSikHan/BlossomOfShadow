#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTT_SetIsInterruptible.generated.h"

UCLASS()
class RPG_API UCBTT_SetIsInterruptible : public UBTTaskNode
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere)
	bool bValue;
public:
	UCBTT_SetIsInterruptible();
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
