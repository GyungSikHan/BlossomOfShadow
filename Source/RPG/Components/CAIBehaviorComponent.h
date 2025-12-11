#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CAIBehaviorComponent.generated.h"

UENUM(BlueprintType)
enum class EAIStateType : uint8
{
	Passive = 0, Attacking, Hitted, Investigating, Seeking, Approach, Dead, Max
	//Wait = 0, Approach, Action, AVoid, Patrol, Hitted, Skill, Investigate, Dead, Max,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAIStateTypeChanged, EAIStateType, InPrevType, EAIStateType, InNewType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_API UCAIBehaviorComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category = "Key")
		FName TargetKey = "Target";
	UPROPERTY(EditAnywhere, Category = "Key")
		FName AIStateTypeKey = "AIState";
	//UPROPERTY(EditAnywhere, Category = "Key")
	//	FName AttackRadiusKey = "AttackRadius";
	//UPROPERTY(EditAnywhere, Category = "Key")
	//	FName DefendRadiusKey = "DefendRadius";
	UPROPERTY(EditAnywhere, Category = "Key")
		FName DistanceToAttackTargetKey = "DistanceToAttackTarget";
	UPROPERTY(EditAnywhere, Category = "Key")
		FName PatrolLocationKey = "Patrol_Location";
	UPROPERTY(EditAnywhere, Category = "Key")
	FName PointOfInterestKey = "PointOfInterest";
	UPROPERTY(EditAnywhere, Category = "Key")
	FName TeamID = "TeamID";
	UPROPERTY(EditAnywhere, Category = "Key")
	FName AvoidLocationKey = "Avoid_Location";

public:
	EAIStateType GetType();

public:
	bool IsPassiveMode();
	bool IsAttackMode();
	bool IsHittedMode();
	bool IsInvestigateMode();
	bool IsSeekingMode();
	bool IsApproachMode();
	bool IsDeadMode();
	
public:
	UCAIBehaviorComponent();

protected:
	virtual void BeginPlay() override;

public:
	FORCEINLINE void SetBlackboard(class UBlackboardComponent* InBlackboard) { Blackboard = InBlackboard; }

public:
	class ACharacter* GetTarget();

public:
	FVector GetPatrolLocation();
	void SetPatrolLocation(const FVector& InLocation);

	//float GetDefendRadius();
	//void SetDefendRadius(const float& InRadius);
	
	float GetDistanceToAttackTarget();
	void SetDistanceToAttackTarget(const float& InDistance);

	FVector GetPointOfInterestLocation();
	void SetPointOfInterestLocation(const FVector& InLocation);

	int32 GetTeamID();
	void SetTeamID(const int32& InTeamID);
	//float GetAttackRadius();
	//void SetAttackRadius(const float& InRange);

	FVector GetAvoidlocation();

public:
	void SetPassiveMode();
	void SetAttackMode();
	void SetHittedMode();
	void SetInvestigateMode();
	void SetSeekingMode();
	void SetApproachMode();
	void SetDeadMode();

	void SetNullTarget();

private:
	void ChangeType(EAIStateType InType);

public:
	FAIStateTypeChanged OnAIStateTypeChanged;

private:
	class UBlackboardComponent* Blackboard;
};


//{
//	GENERATED_BODY()
//private:
//	UPROPERTY(EditAnywhere, Category = "Key")
//	FName AIStateTypeKey = "AIState";
//	UPROPERTY(EditAnywhere, Category = "Key")
//	FName TargetKey = "Target";
//	UPROPERTY(EditAnywhere, Category = "Key")
//	FName PatrolLocationKey = "Patrol_Location";
//	UPROPERTY(EditAnywhere, Category = "Key")
//	FName PointLocationKey = "PointOfInterest";
//	UPROPERTY(EditAnywhere, Category = "Key")
//	FName AISkillLocationKey = "Skill_Location";
//	UPROPERTY(EditAnywhere, Category = "Key")
//	FName AttackRangeKey = "AttackRange";
//	UPROPERTY(EditAnywhere, Category = "Key")
//	FName AISkillsTypeKey = "Skills";
//
//public:
//	EAIStateType GetType();
//	EAISkillType GetSkill();
//
//public:
//
//	bool IsWaitMode();
//	bool IsApproachMode();
//	bool IsActionMode();
//	bool IsAvoid();
//	bool IsPatrolMode();
//	bool IsHittedMode();
//	bool IsInvestigateMode();
//	bool IsSkillMode();
//	bool IsDeadMode();
//
//public:
//	UCAIBehaviorComponent();
//
//protected:
//	virtual void BeginPlay() override;
//
//public:
//	FORCEINLINE void SetBlackboard(class UBlackboardComponent* InBlackboard) { Blackboard = InBlackboard; }
//
//public:
//	class ACharacter* GetTarget();
//	int32 GetTeam(ACharacter* target);
//
//public:
//	FVector GetPatrolLocation();
//	void SetPatrolLocation(const FVector& InLocation);
//
//	FVector GetPointLocation();
//	void SetPointeLocation(const FVector& InLocation);
//
//	FVector GetSkillLocation();
//	void SetSkillLocation(const FVector& InLocation);
//
//	float GetAttackRange();
//	void SetAttackRange(const float& InRange);
//
//public:
//	void SetWaitMode();
//	void SetApproachMode();
//	void SetActionMode();
//	void SetAvoidMode();
//	void SetPatrolMode();
//	void SetHittedMode();
//	void SetSkillMode();
//	void SetInvestigateMode();
//	void SetDeadMode();
//
//	int32 GetMyTeam();
//
//public:
//	void SetSkill1();
//	void SetSkill2();
//	void SetSkill3();
//
//
//private:
//	void ChangeType(EAIStateType InType);
//	void ChangeSkill(EAISkillType InType);
//
//public:
//	FAIStateTypeChanged OnAIStateTypeChanged;
//	FAISkillTypeChanged OnAISkillTypeChanged;
//private:
//	class UBlackboardComponent* Blackboard;
//
//private:
//	int32 TeamId;
//		
//};
