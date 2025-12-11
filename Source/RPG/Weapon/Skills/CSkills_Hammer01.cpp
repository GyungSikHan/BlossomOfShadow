#include "CSkills_Hammer01.h"
#include "RPG/Components/CMovementComponent.h"
#include "RPG/Components/CStateComponent.h"
#include "GameFramework/Character.h"

void UCSkills_Hammer01::Skill_Pressed()
{
	if (Character != nullptr)
	{
		if ((Character->GetMp() - ActionDatas[Index].MP) < 0 || (Character->GetStamina() - ActionDatas[Index].Stamina) < 0)
			return;
	}
	if(State->IsIdleMode() == false)
		return;
	if(State->IsSkillMode() == true)
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

void UCSkills_Hammer01::Begin_Skill_Implementation()
{
	Super::Begin_Skill_Implementation();
	FActorSpawnParameters params;
	params.Owner = Cast<ACharacter>(Owner);
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FTransform transform;
	transform.SetLocation(Owner->GetActorLocation());
	transform.AddToTranslation(ActorLocation);
	transform.SetRotation(FQuat(Owner->GetActorRotation()));

	Owner->GetWorld()->SpawnActor<AActor>(ActorClass, transform, params);
}

void UCSkills_Hammer01::End_Skill_Implementation()
{
	Super::End_Skill_Implementation();
	State->SetIdleMode();
	State->OffSkillMode();
}
