#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBT_UpdateDistanceTarget.generated.h"

UCLASS()
class RPG_API UCBT_UpdateDistanceTarget : public UBTService
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category = "AttackRainge")
	float AttackRainge = 200;
public:
	UCBT_UpdateDistanceTarget();
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
