#include "CSkills_Meteor.h"
#include "RPG/Components/CStateComponent.h"
#include "GameFramework/Character.h"
#include "RPG/Weapon/Add_On/CMeteor.h"
#include "RPG/Characters/AI/CEnemy_AI.h"
#include "RPG/Components/CAIBehaviorComponent.h"
#include "RPG/Utilities/CLog.h"
UCSkills_Meteor::UCSkills_Meteor()
{
}

void UCSkills_Meteor::Skill_Pressed()
{
	if (Character != nullptr)
	{
		if ((Character->GetMp() - ActionDatas[Index].MP) < 0 || (Character->GetStamina() - ActionDatas[Index].Stamina) < 0)
			return;
	}
	if (State->IsIdleMode() == false)
		return;
	if (State->IsSkillMode() == true)
		return;

	Super::Skill_Pressed();


	State->SetActionMode();
	State->OnSkillMode();

	if (Character != nullptr)
	{
		Character->UseMp(ActionDatas[Index].MP);
		Character->UseStamina(ActionDatas[Index].Stamina);
	}
	ActionDatas[Index].DoAction(Owner);
}

void UCSkills_Meteor::Begin_Skill_Implementation()
{
	Super::Begin_Skill_Implementation();

	FActorSpawnParameters params;
	params.Owner = Cast<ACharacter>(Owner);
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FTransform transform;

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(Owner);

	FVector ownerLocation = Owner->GetActorLocation();
	ownerLocation += Owner->GetActorRotation().RotateVector(MeteorLocation);

	if(ai != nullptr)
	{
		UCAIBehaviorComponent* behavior = Cast<UCAIBehaviorComponent>(ai->GetComponentByClass(UCAIBehaviorComponent::StaticClass()));
		ACharacter* target = behavior->GetTarget();
		ownerLocation = FVector(target->GetActorLocation().X, target->GetActorLocation().Y, Owner->GetActorRotation().RotateVector(MeteorLocation).Z);
	}

	transform.SetLocation(ownerLocation);
	Owner->GetWorld()->SpawnActor<AActor>(MeteorClass, transform, params);

}

void UCSkills_Meteor::End_Skill_Implementation()
{
	Super::End_Skill_Implementation();
	
	State->SetIdleMode();
	State->OffSkillMode();
}
