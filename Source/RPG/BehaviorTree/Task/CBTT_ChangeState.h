#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RPG/Components/CAIBehaviorComponent.h"
#include "CBTT_ChangeState.generated.h"

UCLASS()
class RPG_API UCBTT_ChangeState : public UBTTaskNode
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category = "Key")
	EAIStateType Type;
public:
	UCBTT_ChangeState();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
