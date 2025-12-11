#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTT_SetFocus.generated.h"

UCLASS()
class RPG_API UCBTT_SetFocus : public UBTTaskNode
{
	GENERATED_BODY()
//private:
//	UPROPERTY(EditAnywhere, Category = "Keys")
//	FBlackboardKeySelector Key;
public:
	UCBTT_SetFocus();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
