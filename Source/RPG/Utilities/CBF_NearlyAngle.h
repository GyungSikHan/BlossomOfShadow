#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CBF_NearlyAngle.generated.h"

UCLASS()
class RPG_API UCBF_NearlyAngle : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	ACharacter* GetNearlyFrontAngle(ACharacter* InCharacter, TArray<ACharacter*> InArray);
};
