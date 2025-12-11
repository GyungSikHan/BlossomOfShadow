#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CBTD_CanSeeTarget.generated.h"

UCLASS()
class RPG_API UCBTD_CanSeeTarget : public UBTDecorator
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category = "Debug")
	TEnumAsByte<EDrawDebugTrace::Type> Debug;
public:
	UCBTD_CanSeeTarget();
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
