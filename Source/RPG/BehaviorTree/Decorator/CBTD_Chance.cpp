#include "CBTD_Chance.h"
#include "Kismet/KismetMathLibrary.h"

UCBTD_Chance::UCBTD_Chance()
{
	NodeName = "Chance";
}

bool UCBTD_Chance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	return Percent >= (float)UKismetMathLibrary::RandomFloatInRange(0, 1);
}
