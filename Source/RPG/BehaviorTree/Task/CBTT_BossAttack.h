#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RPG/Characters/AI/CEnemy_AI.h"
#include "CBTT_BossAttack.generated.h"

UCLASS()
class RPG_API UCBTT_BossAttack : public UBTTaskNode
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category = "Keys")
	EBossAttacks eBossAttacks;
public:
	UCBTT_BossAttack();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	void AttackBasedOnName(UCWeaponComponent* Weapon);
	void ComboAttack(UCWeaponComponent* Weapon);
protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	bool IsSkill;
};


