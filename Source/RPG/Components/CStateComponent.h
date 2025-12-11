#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStateComponent.generated.h"

UENUM()
enum class EStateType : uint8
{
	Idle = 0, BackStep, Roll_F, Roll_B, Roll_R, Roll_L, Roll_FR, Roll_FL, Roll_BR, Roll_BL, Equip, Action, Hitted, Dead, HitAndDefence, KoandGetup, HitLand, Max
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStateTypeChanged, EStateType, InPrevType, EStateType, InNewType);

UCLASS()
class RPG_API UCStateComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	FORCEINLINE bool IsIdleMode() { return Type == EStateType::Idle; }
	FORCEINLINE bool IsBackStepMode() { return Type == EStateType::BackStep; }
	FORCEINLINE bool IsRoll_FMode() { return Type == EStateType::Roll_F; }
	FORCEINLINE bool IsRoll_BMode() { return Type == EStateType::Roll_B; }
	FORCEINLINE bool IsRoll_RMode() { return Type == EStateType::Roll_R; }
	FORCEINLINE bool IsRoll_LMode() { return Type == EStateType::Roll_L; }
	FORCEINLINE bool IsRoll_FRMode() { return Type == EStateType::Roll_FR; }
	FORCEINLINE bool IsRoll_FLMode() { return Type == EStateType::Roll_FL; }
	FORCEINLINE bool IsRoll_BRMode() { return Type == EStateType::Roll_BR; }
	FORCEINLINE bool IsRoll_BLMode() { return Type == EStateType::Roll_BL; }
	FORCEINLINE bool IsEquipMode() { return Type == EStateType::Equip; }
	FORCEINLINE bool IsActionMode() { return Type == EStateType::Action; }
	FORCEINLINE bool IsHittdMode() { return Type == EStateType::Hitted; }
	FORCEINLINE bool IsDeadMode() { return Type == EStateType::Dead; }

	FORCEINLINE bool IsSkillMode() { return bInSkillMode; }


	FORCEINLINE EStateType PreveType() { return PrevType; }
	FORCEINLINE bool IsInterruptible() { return bInterruptible; }

public:	
	UCStateComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetIdleMode();
	void SetBackStepMode();
	void SetRoll_FB_Mode(int32 InIndex);
	void SetRoll_RL_Mode(int32 InIndex);
	void SetRoll_FRL_Mode(int32 InIndex);
	void SetRoll_BRL_Mode(int32 InIndex);
	void SetEquipMode();
	void SetActionMode();
	void SetHittednMode();
	void SetDeadMode();
	//void SetDefenceMode(bool InIndex);
	void SetInterruptible(bool InIndex);

	void OnSkillMode();
	void OffSkillMode();

public:
	void ChangeType(EStateType NewType);

public:
	FStateTypeChanged OnStateTypeChanged;

private:
	EStateType Type;
	EStateType PrevType;
	bool bInSkillMode;
	bool bInterruptible;
};
