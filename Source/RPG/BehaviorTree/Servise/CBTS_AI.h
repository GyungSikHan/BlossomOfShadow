#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTS_AI.generated.h"

UCLASS()
class RPG_API UCBTS_AI : public UBTService
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category = "Action")
	float ActionRange = 500;
	UPROPERTY(EditAnywhere, Category = "Action")
	bool bDrawDebug;

public:
	UCBTS_AI();
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
