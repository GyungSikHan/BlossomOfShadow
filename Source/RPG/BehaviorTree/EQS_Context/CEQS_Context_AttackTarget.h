#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "CEQS_Context_AttackTarget.generated.h"

UCLASS(Blueprintable)
class RPG_API UCEQS_Context_AttackTarget : public UEnvQueryContext
{
	GENERATED_BODY()
private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> Actor;
	
public:
	UCEQS_Context_AttackTarget();
	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;
};
