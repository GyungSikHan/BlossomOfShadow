#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "CBTD_Chance.generated.h"

UCLASS()
class RPG_API UCBTD_Chance : public UBTDecorator
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere)
	float Percent;

public:
	UCBTD_Chance();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
