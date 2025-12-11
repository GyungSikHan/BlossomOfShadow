#include "CStateComponent.h"

UCStateComponent::UCStateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCStateComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCStateComponent::SetIdleMode()
{
	ChangeType(EStateType::Idle);
}

void UCStateComponent::SetBackStepMode()
{
	ChangeType(EStateType::BackStep);
}

void UCStateComponent::SetRoll_FB_Mode(int32 InIndex)
{
	if (InIndex == 0)
	{
		ChangeType(EStateType::Roll_B);
		return;
	}
	ChangeType(EStateType::Roll_F);

}

void UCStateComponent::SetRoll_RL_Mode(int32 InIndex)
{
	if (InIndex == 2)
	{
		ChangeType(EStateType::Roll_L);
		return;
	}

	ChangeType(EStateType::Roll_R);
}

void UCStateComponent::SetRoll_FRL_Mode(int32 InIndex)
{
	if (InIndex == 4)
	{
		ChangeType(EStateType::Roll_FL);
		return;
	}

	ChangeType(EStateType::Roll_FR);
}

void UCStateComponent::SetRoll_BRL_Mode(int32 InIndex)
{
	if (InIndex == 6)
	{
		ChangeType(EStateType::Roll_BL);
		return;
	}

	ChangeType(EStateType::Roll_BR);
}

void UCStateComponent::SetEquipMode()
{
	ChangeType(EStateType::Equip);
}

void UCStateComponent::SetActionMode()
{
	ChangeType(EStateType::Action);
}

void UCStateComponent::SetHittednMode()
{
	ChangeType(EStateType::Hitted);
}

void UCStateComponent::SetDeadMode()
{
	ChangeType(EStateType::Dead);
}

void UCStateComponent::SetInterruptible(bool InIndex)
{
	bInterruptible = InIndex;
}

void UCStateComponent::OnSkillMode()
{
	bInSkillMode = true;
}

void UCStateComponent::OffSkillMode()
{
	bInSkillMode = false;
}

void UCStateComponent::ChangeType(EStateType NewType)
{
	PrevType = Type;
	Type = NewType;

	if (OnStateTypeChanged.IsBound())
		OnStateTypeChanged.Broadcast(PrevType, Type);
}

