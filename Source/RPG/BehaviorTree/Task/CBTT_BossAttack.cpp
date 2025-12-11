#include "CBTT_BossAttack.h"
#include "RPG/Utilities/CLog.h"
#include "RPG/Characters/AI/CAIController.h"
#include "RPG/Components/CWeaponComponent.h"
#include "RPG/Weapon/CDoAction.h"
#include "RPG/Weapon/DoActions/CDoAction_Combo.h"
#include "RPG/Weapon/DoActions/CDoAction_Warp.h"
#include "RPG/Weapon/Skills/CSkills.h"

UCBTT_BossAttack::UCBTT_BossAttack()
{
	bNotifyTick = true;
	NodeName = "Boss Attack";
}

EBTNodeResult::Type UCBTT_BossAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	if (ai == nullptr)
		return EBTNodeResult::Failed;

	UCWeaponComponent* weapon = Cast<UCWeaponComponent>(ai->GetComponentByClass(UCWeaponComponent::StaticClass()));
	if (weapon == nullptr)
		return EBTNodeResult::Failed;

	AttackBasedOnName(weapon);

	return EBTNodeResult::InProgress;
}

void UCBTT_BossAttack::AttackBasedOnName(UCWeaponComponent* Weapon)
{
	switch (eBossAttacks)
	{
	case EBossAttacks::Combo1:
		Weapon->DoAction();
		IsSkill = false;
		break;
	case EBossAttacks::Projectile1:
		Weapon->Skills_Pressed(ESkillsType::Skill1);
		break;
	case EBossAttacks::Projectile2:
		Weapon->Skills_Pressed(ESkillsType::Skill2);
		IsSkill = true;
		break;
	case EBossAttacks::AerialAttack:
		Weapon->Skills_Pressed(ESkillsType::Skill3);
		IsSkill = true;
		break;
	case EBossAttacks::GrundSmash:
		Weapon->Skills_Pressed(ESkillsType::Skill4);
		IsSkill = true;
		break;
	case EBossAttacks::AOETrail:
		Weapon->Skills_Pressed(ESkillsType::Skill5);
		IsSkill = true;
		break;
	case EBossAttacks::Warp:
		Weapon->DoAction();
		IsSkill = false;
		break;
	}
}

void UCBTT_BossAttack::ComboAttack(UCWeaponComponent* Weapon)
{
	UCDoAction_Combo* combo = Cast<UCDoAction_Combo>(Weapon->GetDoAction());
	combo->EnableCombo();
	combo->Begin_DoAction();
}

void UCBTT_BossAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	UCWeaponComponent* weapon = Cast<UCWeaponComponent>(ai->GetComponentByClass(UCWeaponComponent::StaticClass()));
	UCStateComponent* state = Cast<UCStateComponent>(ai->GetComponentByClass(UCStateComponent::StaticClass()));

	bool bCheck = true;
	bCheck &= state->IsIdleMode();
	if (IsSkill == true)
		bCheck &= weapon->GetSkills()->GetInAction() == false;
	else
		bCheck &= weapon->GetDoAction()->GetInAction() == false;

	if(bCheck == true)
	{
		if(IsSkill == true)
		{
			weapon->GetSkills()->End_Skill_Implementation();
			weapon->Skills_Released();
		}
		
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
}

EBTNodeResult::Type UCBTT_BossAttack::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	if (controller == nullptr || ai == nullptr)
		return EBTNodeResult::Failed;

	UCStateComponent* state = Cast<UCStateComponent>(ai->GetComponentByClass(UCStateComponent::StaticClass()));
	if (state == nullptr)
		return EBTNodeResult::Failed;
	if (state->IsDeadMode() == true)
	{
		return EBTNodeResult::Aborted;
	}

	UCWeaponComponent* weapon = Cast<UCWeaponComponent>(ai->GetComponentByClass(UCWeaponComponent::StaticClass()));
	if (weapon == nullptr)
		return EBTNodeResult::Failed;

	bool bBeginAction{};
	if(IsSkill == true)
	{
		bBeginAction = weapon->GetSkills()->GetInAction();
		if (bBeginAction == false)
			weapon->GetSkills()->Begin_Skill_Implementation();
		weapon->GetSkills()->End_Skill_Implementation();
		weapon->Skills_Released();
	}
	else
	{
		bBeginAction = weapon->GetDoAction()->GetBeginAction();
		if (bBeginAction == false)
			weapon->GetDoAction()->Begin_DoAction();

		weapon->GetDoAction()->End_DoAction();
	}

	return EBTNodeResult::Succeeded;
}