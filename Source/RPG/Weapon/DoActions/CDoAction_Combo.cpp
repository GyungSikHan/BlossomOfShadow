#include "CDoAction_Combo.h"
#include "AIController.h"
#include "RPG/Utilities/CLog.h"
#include "RPG/Components/CStateComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"

void UCDoAction_Combo::DoAction()
{
	if (DoActionDatas.Num() < 1)
		return;

	if (character != nullptr && Index == 0)
	{
		if ((character->GetMp() - DoActionDatas[Index].MP) < 0 || (character->GetStamina() - DoActionDatas[Index].Stamina) < 0)
			return;
	}

	if(bEnable == true)
	{
		bEnable = false;
		bExsit = true;

		return;
	}

	if (State->IsIdleMode() == false)
		return;

	Super::DoAction();
	
	if(character != nullptr)
	{
		character->UseMp(DoActionDatas[Index].MP);
		character->UseStamina(DoActionDatas[Index].Stamina);
	}
	DoActionDatas[Index].DoAction(OwnerCharacter);
}

void UCDoAction_Combo::Begin_DoAction()
{
	Super::Begin_DoAction();

	AAIController* controller = OwnerCharacter->GetController<AAIController>();
	if (controller != nullptr)
	{
		float random = (float)UKismetMathLibrary::RandomFloatInRange(0, 1);
		if (RandomIndex <= random)
			bExsit = true;
		
	}

	if(bExsit == false)
		return;

	if (character != nullptr)
	{
		if ((character->GetMp() - DoActionDatas[Index].MP) < 0 || (character->GetStamina() - DoActionDatas[Index].Stamina) < 0)
		{
			Index = 0;
			bExsit = false;
			return;
		}
	}

	bExsit = false;
	Index++;

	if (character != nullptr)
	{
		character->UseMp(DoActionDatas[Index].MP);
		character->UseStamina(DoActionDatas[Index].Stamina);
	}

	DoActionDatas[Index].DoAction(OwnerCharacter);

}

void UCDoAction_Combo::End_DoAction()
{
	Super::End_DoAction();

	Index = 0;
}

void UCDoAction_Combo::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCuaser, ACharacter* InOther)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InAttackCuaser, InOther);
	if(InOther == nullptr)
		return;

	for (ACharacter* hitted : Hitted)
		if(hitted == InOther)
			return;

	Hitted.AddUnique(InOther);

	if(HitDatas.Num() -1 < Index)
		return;

	HitDatas[Index].SendDamage(InAttacker, InAttackCuaser, InOther);
}

void UCDoAction_Combo::OnAttachmentEndCollision()
{
	Super::OnAttachmentEndCollision();
	Hitted.Empty();
}
