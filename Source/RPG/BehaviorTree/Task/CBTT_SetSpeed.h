#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RPG/Components/CMovementComponent.h"
#include "CBTT_SetSpeed.generated.h"

UCLASS()
class RPG_API UCBTT_SetSpeed : public UBTTaskNode
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category = "Type")
	ESpeedType Type;
public:
	UCBTT_SetSpeed();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
