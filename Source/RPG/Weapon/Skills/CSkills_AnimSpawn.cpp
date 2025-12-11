#include "CSkills_AnimSpawn.h"
#include "RPG/Components/CMovementComponent.h"
#include "RPG/Components/CStateComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"

void UCSkills_AnimSpawn::Skill_Pressed()
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

void UCSkills_AnimSpawn::Begin_Skill_Implementation()
{
	Super::Begin_Skill_Implementation();
	int32 index = UKismetMathLibrary::RandomFloatInRange(0, ActorClass.Num() - 1);

	FActorSpawnParameters params;
	params.Owner = Cast<ACharacter>(Owner);
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	Owner->GetWorld()->SpawnActor<AActor>(ActorClass[index], params);
}

void UCSkills_AnimSpawn::End_Skill_Implementation()
{
	Super::End_Skill_Implementation();
	State->SetIdleMode();
	State->OffSkillMode();
}
