#include "CAIBehaviorComponent.h"
#include "RPG/Utilities/CLog.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "RPG/Characters/CPlayer.h"
#include "RPG/Characters/AI/CEnemy_AI.h"

EAIStateType UCAIBehaviorComponent::GetType()
{
	return (EAIStateType)Blackboard->GetValueAsEnum(AIStateTypeKey);
}

bool UCAIBehaviorComponent::IsPassiveMode()
{
	return GetType() == EAIStateType::Passive;
}

bool UCAIBehaviorComponent::IsAttackMode()
{
	return GetType() == EAIStateType::Attacking;
}

bool UCAIBehaviorComponent::IsHittedMode()
{
	return GetType() == EAIStateType::Hitted;
}

bool UCAIBehaviorComponent::IsInvestigateMode()
{
	return GetType() == EAIStateType::Investigating;
}

bool UCAIBehaviorComponent::IsSeekingMode()
{
	return GetType() == EAIStateType::Seeking;
}

bool UCAIBehaviorComponent::IsApproachMode()
{
	return GetType() == EAIStateType::Approach;
}

bool UCAIBehaviorComponent::IsDeadMode()
{
	return GetType() == EAIStateType::Dead;
}

UCAIBehaviorComponent::UCAIBehaviorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCAIBehaviorComponent::BeginPlay()
{
	Super::BeginPlay();

	//ACharacter* character = Cast<ACharacter>(GetOwner());
	//ACEnemy_AI* ai = Cast<ACEnemy_AI>(character);
	//if (ai != nullptr)
	//	TeamId = ai->GetTeamID();
	//else
	//{
	//	ACEnemy_Boss* boss = Cast<ACEnemy_Boss>(character);
	//	TeamId = boss->GetTeamID();
	//}
}

ACharacter* UCAIBehaviorComponent::GetTarget()
{
	return Cast<ACharacter>(Blackboard->GetValueAsObject(TargetKey));
}

FVector UCAIBehaviorComponent::GetPatrolLocation()
{
	return Blackboard->GetValueAsVector(PatrolLocationKey);
}

void UCAIBehaviorComponent::SetPatrolLocation(const FVector& InLocation)
{
	Blackboard->SetValueAsVector(PatrolLocationKey, InLocation);
}

//float UCAIBehaviorComponent::GetDefendRadius()
//{
//	return Blackboard->GetValueAsFloat(DefendRadiusKey);
//}
//
//void UCAIBehaviorComponent::SetDefendRadius(const float& InRadius)
//{
//	Blackboard->SetValueAsFloat(DefendRadiusKey, InRadius);
//}

float UCAIBehaviorComponent::GetDistanceToAttackTarget()
{
	return Blackboard->GetValueAsFloat(DistanceToAttackTargetKey);
}

void UCAIBehaviorComponent::SetDistanceToAttackTarget(const float& InDistance)
{
	Blackboard->SetValueAsFloat(DistanceToAttackTargetKey, InDistance);
}

FVector UCAIBehaviorComponent::GetPointOfInterestLocation()
{
	return Blackboard->GetValueAsVector(PointOfInterestKey);
}

void UCAIBehaviorComponent::SetPointOfInterestLocation(const FVector& InLocation)
{
	Blackboard->SetValueAsVector(PointOfInterestKey, InLocation);
}

int32 UCAIBehaviorComponent::GetTeamID()
{
	return Blackboard->GetValueAsInt(TeamID);
}

void UCAIBehaviorComponent::SetTeamID(const int32& InTeamID)
{
	Blackboard->SetValueAsInt(TeamID, InTeamID);
}

FVector UCAIBehaviorComponent::GetAvoidlocation()
{
	return Blackboard->GetValueAsVector(AvoidLocationKey);
}

//float UCAIBehaviorComponent::GetAttackRadius()
//{
//	return Blackboard->GetValueAsFloat(AttackRadiusKey);
//}
//
//void UCAIBehaviorComponent::SetAttackRadius(const float& InRange)
//{
//	Blackboard->SetValueAsFloat(AttackRadiusKey, InRange);
//}

void UCAIBehaviorComponent::SetPassiveMode()
{
	ChangeType(EAIStateType::Passive);
}

void UCAIBehaviorComponent::SetAttackMode()
{
	ChangeType(EAIStateType::Attacking);
}

void UCAIBehaviorComponent::SetHittedMode()
{
	ChangeType(EAIStateType::Hitted);
}

void UCAIBehaviorComponent::SetInvestigateMode()
{
	ChangeType(EAIStateType::Investigating);
}

void UCAIBehaviorComponent::SetSeekingMode()
{
	ChangeType(EAIStateType::Seeking);
}

void UCAIBehaviorComponent::SetApproachMode()
{
	ChangeType(EAIStateType::Approach);
}

void UCAIBehaviorComponent::SetDeadMode()
{
	ChangeType(EAIStateType::Dead);
}

void UCAIBehaviorComponent::ChangeType(EAIStateType InType)
{
	EAIStateType prevType = GetType();

	Blackboard->SetValueAsEnum(AIStateTypeKey, (uint8)InType);

	if (OnAIStateTypeChanged.IsBound())
		OnAIStateTypeChanged.Broadcast(prevType, InType);
}



//void UCAIBehaviorComponent::BeginPlay()
//{
//	Super::BeginPlay();
//	ACharacter* character = Cast<ACharacter>(GetOwner());
//	ACEnemy_AI* ai = Cast<ACEnemy_AI>(character);
//	if (ai != nullptr)
//		TeamId = ai->GetTeamID();
//	else
//	{
//		ACEnemy_Boss* boss = Cast<ACEnemy_Boss>(character);
//		TeamId = boss->GetTeamID();
//	}
//}
//
//EAIStateType UCAIBehaviorComponent::GetType()
//{
//	return (EAIStateType)Blackboard->GetValueAsEnum(AIStateTypeKey);
//}
//
//EAISkillType UCAIBehaviorComponent::GetSkill()
//{
//	return(EAISkillType)Blackboard->GetValueAsEnum(AISkillsTypeKey);
//}
//
//bool UCAIBehaviorComponent::IsWaitMode()
//{
//	return GetType() == EAIStateType::Wait;
//}
//
//bool UCAIBehaviorComponent::IsApproachMode()
//{
//	return GetType() == EAIStateType::Approach;
//}
//
//bool UCAIBehaviorComponent::IsActionMode()
//{
//	return GetType() == EAIStateType::Action;
//}
//
//bool UCAIBehaviorComponent::IsAvoid()
//{
//	return GetType() == EAIStateType::AVoid;
//}
//
//bool UCAIBehaviorComponent::IsPatrolMode()
//{
//	return GetType() == EAIStateType::Patrol;
//}
//
//bool UCAIBehaviorComponent::IsHittedMode()
//{
//	return GetType() == EAIStateType::Hitted;
//}
//
//bool UCAIBehaviorComponent::IsInvestigateMode()
//{
//	return GetType() == EAIStateType::Investigate;
//}
//
//bool UCAIBehaviorComponent::IsSkillMode()
//{
//	return GetType() == EAIStateType::Skill;
//}
//
////bool UCAIBehaviorComponent::IsSkillMode()
////{
////	return GetType() == EAIStateType::Skill;
////}
//
//bool UCAIBehaviorComponent::IsDeadMode()
//{
//	return GetType() == EAIStateType::Dead;
//}
//
//ACharacter* UCAIBehaviorComponent::GetTarget()
//{
//	return Cast<ACharacter>(Blackboard->GetValueAsObject(TargetKey));
//}
//
//int32 UCAIBehaviorComponent::GetTeam(ACharacter* target)
//{
//	ACPlayer* player = Cast<ACPlayer>(target);
//	ACEnemy_AI* ai = Cast<ACEnemy_AI>(target);
//	ACEnemy_Boss* boss = Cast<ACEnemy_Boss>(target);
//	if (player == nullptr && boss == nullptr && ai != nullptr)
//		return ai->GetTeamID();
//	if (ai == nullptr && boss == nullptr && player != nullptr)
//		return player->GetTeam();
//	if (ai == nullptr && player == nullptr && boss != nullptr)
//		return boss->GetTeamID();
//	return 0;
//}
//
//FVector UCAIBehaviorComponent::GetPatrolLocation()
//{
//	return Blackboard->GetValueAsVector(PatrolLocationKey);
//}
//
//void UCAIBehaviorComponent::SetPatrolLocation(const FVector& InLocation)
//{
//	Blackboard->SetValueAsVector(PatrolLocationKey, InLocation);
//}
//
//FVector UCAIBehaviorComponent::GetPointLocation()
//{
//	return Blackboard->GetValueAsVector(PointLocationKey);
//}
//
//void UCAIBehaviorComponent::SetPointeLocation(const FVector& InLocation)
//{
//	Blackboard->SetValueAsVector(PointLocationKey, InLocation);
//}
//
//FVector UCAIBehaviorComponent::GetSkillLocation()
//{
//	return Blackboard->GetValueAsVector(AISkillLocationKey);
//}
//
//void UCAIBehaviorComponent::SetSkillLocation(const FVector& InLocation)
//{
//	Blackboard->SetValueAsVector(AISkillLocationKey, InLocation);
//}
//
//float UCAIBehaviorComponent::GetAttackRange()
//{
//	return Blackboard->GetValueAsFloat(AttackRangeKey);
//}
//
//void UCAIBehaviorComponent::SetAttackRange(const float& InRange)
//{
//	Blackboard->SetValueAsFloat(AttackRangeKey, InRange);
//}
//
//
//void UCAIBehaviorComponent::SetWaitMode()
//{
//	ChangeType(EAIStateType::Wait);
//}
//
//void UCAIBehaviorComponent::SetApproachMode()
//{
//	ChangeType(EAIStateType::Approach);
//}
//
//void UCAIBehaviorComponent::SetActionMode()
//{
//	ChangeType(EAIStateType::Action);
//}
//
//void UCAIBehaviorComponent::SetAvoidMode()
//{
//	ChangeType(EAIStateType::AVoid);
//}
//
//void UCAIBehaviorComponent::SetPatrolMode()
//{
//	ChangeType(EAIStateType::Patrol);
//}
//
//void UCAIBehaviorComponent::SetHittedMode()
//{
//	ChangeType(EAIStateType::Hitted);
//}
//
//void UCAIBehaviorComponent::SetSkillMode()
//{
//	ChangeType(EAIStateType::Skill);
//}
//
//void UCAIBehaviorComponent::SetInvestigateMode()
//{
//	ChangeType(EAIStateType::Investigate);
//}
//
//void UCAIBehaviorComponent::SetDeadMode()
//{
//	ChangeType(EAIStateType::Dead);
//}
//
//int32 UCAIBehaviorComponent::GetMyTeam()
//{
//	return TeamId;
//}
//
//void UCAIBehaviorComponent::SetSkill1()
//{
//	ChangeSkill(EAISkillType::Skill1);
//}
//
//void UCAIBehaviorComponent::SetSkill2()
//{
//	ChangeSkill(EAISkillType::Skill2);
//}
//
//void UCAIBehaviorComponent::SetSkill3()
//{
//	ChangeSkill(EAISkillType::Skill3);
//}
//
//void UCAIBehaviorComponent::ChangeType(EAIStateType InType)
//{
//	EAIStateType prevType = GetType();
//
//	Blackboard->SetValueAsEnum(AIStateTypeKey, (uint8)InType);
//
//	if (OnAIStateTypeChanged.IsBound())
//		OnAIStateTypeChanged.Broadcast(prevType, InType);
//}
//
//void UCAIBehaviorComponent::ChangeSkill(EAISkillType InType)
//{
//	EAISkillType prev = GetSkill();
//	Blackboard->SetValueAsEnum(AISkillsTypeKey, (uint8)InType);
//	if (OnAISkillTypeChanged.IsBound())
//		OnAISkillTypeChanged.Broadcast(prev, InType);
//}
