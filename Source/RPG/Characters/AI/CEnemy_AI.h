#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Characters/CCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CEnemy_AI.generated.h"

UENUM()
enum class EBossAttacks : uint8
{
	Combo1 = 0, Projectile1, Projectile2, AerialAttack, GrundSmash, AOETrail, Warp, Max
};

UCLASS()
class RPG_API ACEnemy_AI : public ACCharacter
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACDrop_Item> DropItem;

private:
	UPROPERTY(EditAnywhere, Category = "Trace")
	TEnumAsByte<EDrawDebugTrace::Type> DropItemDebug;
	UPROPERTY(EditAnywhere)
	bool bBoss;


private:
	UPROPERTY(VisibleDefaultsOnly, Category = "UI")
	class UWidgetComponent* Widget;
	UPROPERTY(EditAnywhere, Category = "AI")
	class UBehaviorTree* BehaviorTree;

private:
	UPROPERTY(EditAnywhere, Category = "Key")
	UParticleSystem* Effect;

private:
	UPROPERTY(EditAnywhere, Category = "Patrol")
	class ACPatrolPath* PatrolPath;

private:
	UPROPERTY(VisibleDefaultsOnly)
	class UCAIBehaviorComponent* Behavior;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	float LabelViewAmount = 3000.0f;

public:
	FORCEINLINE class UBehaviorTree* GetBehaviorTree() { return BehaviorTree; }
	FORCEINLINE class ACPatrolPath* GetPatrolPath() { return PatrolPath; }

	FORCEINLINE class ACAIController* GetAIController() { return AIController; }
	
public:
	ACEnemy_AI();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void End_Dead() override;

private:
	void UpdateLabelRenderScale();

protected:
	void Drop_Item();
	virtual void Dead() override;

private:
	uint8 Team;
	bool bIsTeleporting;

	UParticleSystemComponent* particle;
	bool bTarget;
	class ACAIController* AIController;
	class ACPlayer* Target;

	bool bTest;
	//public:
//	UFUNCTION(BlueprintCallable)
//	void SetHammerMode();
//	UFUNCTION(BlueprintCallable)
//	void SetRodMode();
//

};



