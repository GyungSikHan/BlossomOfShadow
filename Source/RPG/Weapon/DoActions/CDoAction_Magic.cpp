#include "CDoAction_Magic.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "RPG/Components/CStateComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "RPG/Weapon/Add_On/CMagicBall.h"

#include "RPG/Characters/AI/CAIController.h"

//UCDoAction_Magic::UCDoAction_Magic()
//{
//}
//
//void UCDoAction_Magic::DoAction()
//{
//	if(DoActionDatas.Num() < 0)
//		return;
//
//	if (bEnable == true)
//	{
//		bEnable = false;
//		bExsit = true;
//		return;
//	}
//
//	if(State->IsIdleMode() == false)
//		return;
//
//	Super::DoAction();
//
//	MagicBall = NewObject<ACMagicBall>(MagicBallClass);
//	DoActionDatas[Index].DoAction(OwnerCharacter);
//
//}
//
//void UCDoAction_Magic::Begin_DoAction()
//{
//	UCameraComponent* camera = OwnerCharacter->GetComponentByClass<UCameraComponent>();
//
//	FRotator rotator;
//	FVector direction;
//	if (!!camera)
//	{
//		FVector forward = camera->GetForwardVector();
//		rotator = UKismetMathLibrary::MakeRotFromX(forward);
//		rotator = UKismetMathLibrary::MakeRotator(0, 0, rotator.Yaw);
//		direction = UKismetMathLibrary::GetForwardVector(rotator);
//	}
//	else
//		direction = OwnerCharacter->GetActorForwardVector();
//	
//	FVector location = OwnerCharacter->GetMesh()->GetSocketLocation("Hand_Rod");
//
//	FActorSpawnParameters params;
//	params.Owner = OwnerCharacter;
//	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
//
//	FTransform transform = UKismetMathLibrary::MakeTransform(location, direction.Rotation(), FVector(1, 1, 1));
//	OwnerCharacter->GetWorld()->SpawnActor<ACMagicBall>(MagicBallClass, transform, params);
//
//	Super::Begin_DoAction();
//	//if(MagicBall == nullptr)
//	//	return;
//	MagicBall->Shoot(direction);
//
//	if(bExsit == false)
//		return;
//
//	bExsit = false;
//	Index++;
//	DoActionDatas[Index].DoAction(OwnerCharacter);
//}
//
//void UCDoAction_Magic::End_DoAction()
//{
//	Super::End_DoAction();
//	Index = 0;
//}

UCDoAction_Magic::UCDoAction_Magic()
{

}

void UCDoAction_Magic::DoAction()
{
	if(DoActionDatas.Num() <= 0)
		return;

	if (character != nullptr && Index == 0)
	{
		if ((character->GetMp() - DoActionDatas[Index].MP) < 0 || (character->GetStamina() - DoActionDatas[Index].Stamina) < 0)
			return;
	}

	if (bEnable == true)
	{
		bEnable = false;
		bExsit = true;
		return;
	}

	if(State->IsIdleMode() == false)
		return;

	Super::DoAction();
	if (character != nullptr)
	{
		character->UseMp(DoActionDatas[Index].MP);
		character->UseStamina(DoActionDatas[Index].Stamina);
	}
	DoActionDatas[Index].DoAction(OwnerCharacter);
}

void UCDoAction_Magic::Begin_DoAction()
{
	Super::Begin_DoAction();

	UCameraComponent* camera = OwnerCharacter->GetComponentByClass<UCameraComponent>();
	bool isEnemy{};
	FVector direction{};
	if (camera != nullptr)
	{
		direction = camera->GetForwardVector();
		FRotator rotator = UKismetMathLibrary::MakeRotFromX(direction);
		rotator = UKismetMathLibrary::MakeRotator(0.0f, 0.0f, rotator.Yaw);
		direction = UKismetMathLibrary::GetForwardVector(rotator);
	}
	else
	{
		direction = OwnerCharacter->GetActorForwardVector();
		isEnemy = true;
	}

	FVector location = OwnerCharacter->GetMesh()->GetSocketLocation("Hand_Rod");

	FActorSpawnParameters param{};
	param.Owner = OwnerCharacter;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FTransform transform = UKismetMathLibrary::MakeTransform(location, direction.Rotation());

	OwnerCharacter->GetWorld()->SpawnActor<ACMagicBall>(MagicBallClass, transform, param);
	if (isEnemy == true)
	{
		int32 randomindex = FMath::RandRange(0, 1);
		if(randomindex == 1)
		{
			if (Index < DoActionDatas.Num() - 1)
			{
				isEnemy = false;
				bExsit = true;
			}
		}
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

void UCDoAction_Magic::End_DoAction()
{
	Super::End_DoAction();

	Index = 0;
}
