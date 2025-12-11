// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "CBTD_IsEquip.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UCBTD_IsEquip : public UBTDecorator
{
	GENERATED_BODY()
public:
	UCBTD_IsEquip();
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};


