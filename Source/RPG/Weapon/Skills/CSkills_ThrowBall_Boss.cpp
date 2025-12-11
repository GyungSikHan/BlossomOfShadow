#include "CSkills_ThrowBall_Boss.h"
#include "RPG/Components/CStateComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "RPG/Weapon/Add_On/CMagicBall.h"

void UCSkills_ThrowBall_Boss::Skill_Pressed()
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

	MagicBall = NewObject<ACMagicBall>(MagicBallClass);

	if (Character != nullptr)
	{
		Character->UseMp(ActionDatas[Index].MP);
		Character->UseStamina(ActionDatas[Index].Stamina);
	}

	ActionDatas[Index].DoAction(Owner);
}

void UCSkills_ThrowBall_Boss::Begin_Skill_Implementation()
{
	FVector direction = Owner->GetActorForwardVector();
	FVector location = Owner->GetMesh()->GetSocketLocation("Hand_Rod");

	FActorSpawnParameters params;
	params.Owner = Owner;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FTransform transform = UKismetMathLibrary::MakeTransform(location, direction.Rotation(), FVector(1, 1, 1));
	Owner->GetWorld()->SpawnActor<ACMagicBall>(MagicBallClass, transform, params);

	Super::Begin_Skill_Implementation();
	MagicBall->Shoot(direction);
}

void UCSkills_ThrowBall_Boss::End_Skill_Implementation()
{
	Super::End_Skill_Implementation();

	State->SetIdleMode();
	State->OffSkillMode();
}
