#include "CSkills_Arrow.h"
#include "RPG/Components/CStateComponent.h"
#include "GameFramework/Character.h"
#include "RPG/Weapon/Add_On/CSkillArrow.h"

UCSkills_Arrow::UCSkills_Arrow()
{
}

void UCSkills_Arrow::Skill_Pressed()
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

void UCSkills_Arrow::Begin_Skill_Implementation()
{
	Super::Begin_Skill_Implementation();
	FActorSpawnParameters params;
	params.Owner = Cast<ACharacter>(Owner);
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FTransform transform;
	FVector ownerLocation = Owner->GetActorLocation();
	ownerLocation += Owner->GetActorRotation().RotateVector(ArrowLocation);
	transform.SetLocation(ownerLocation);
	//transform.SetLocation(Owner->GetActorLocation());
	//transform.AddToTranslation(ArrowLocation);
	transform.SetRotation(FQuat(Owner->GetActorRotation()));

	Owner->GetWorld()->SpawnActor<AActor>(ArrowClass, transform, params);

}

void UCSkills_Arrow::End_Skill_Implementation()
{
	Super::End_Skill_Implementation();
	State->SetIdleMode();
	State->OffSkillMode();
}