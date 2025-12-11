#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "CBTD_IsHealthBelowThreshold.generated.h"

UCLASS()
class RPG_API UCBTD_IsHealthBelowThreshold : public UBTDecorator
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere)
	float HealthThreshold;

public:
	UCBTD_IsHealthBelowThreshold();
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
