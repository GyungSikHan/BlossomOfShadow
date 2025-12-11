// Fill out your copyright notice in the Description page of Project Settings.


#include "CSkills_AirCombo.h"

#include "Camera/CameraComponent.h"
#include "Characters/CPlayer.h"
#include "Components/ArrowComponent.h"
#include "Utilities/CLog.h"

#include "Engine/OverlapResult.h"
#include "Components/CStateComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Weapon/CAttachment.h"
#include "Weapon/CDoAction.h"

UCSkills_AirCombo::UCSkills_AirCombo()
{
}

void UCSkills_AirCombo::Skill_Pressed()
{
	//if(bTest2 == false)
	//{
	//	ACPlayer* p = Cast<ACPlayer>(Owner);
	//	p->SwitchCamera();
	//	bTest2 = true;
	//}
	if (ActionDatas.Num() < 1 && Index > ActionDatas.Num() - 1)
		return;

	if (Character != nullptr && Index == 0)
	{
		if ((Character->GetMp() - ActionDatas[Index].MP) < 0 || (Character->GetStamina() - ActionDatas[Index].Stamina) < 0)
			return;
	}

	if (bEnable == true)
	{
		bEnable = false;
		bExsit = true;
		return;
	}
	if (State->IsIdleMode() == false)
		return; 
	if (State->IsSkillMode() == true)
		return;


	Super::Skill_Pressed();
	State->SetActionMode();
	State->OnSkillMode();

	//if (Character != nullptr)
	//{
	//	Character->UseMp(ActionDatas[Index].MP);
	//	Character->UseStamina(ActionDatas[Index].Stamina);
	//}

	Attachment->OnAttachmentEndCollision.Remove(DoAction, "OnAttachmentEndCollision");
	Attachment->OnAttachmentBeginOverlap.Remove(DoAction, "OnAttachmentBeginOverlap");

	//Attachment->OnAttachmentEndCollision.AddDynamic(this, &UCSkills_AirCombo::OnAttachmentEndCollision);
	//Attachment->OnAttachmentBeginOverlap.AddDynamic(this, &UCSkills_AirCombo::OnAttachmentBeginOverlap);

	ActionDatas[Index].DoAction(Owner);
}

void UCSkills_AirCombo::Begin_Skill_Implementation()
{
	if (Index > ActionDatas.Num() - 1)
		return;
	Super::Begin_Skill_Implementation();

	DrawCollision();

	if (bExsit == false)
		return;

	if (Character != nullptr)
	{
		if ((Character->GetMp() - ActionDatas[Index].MP) < 0 || (Character->GetStamina() - ActionDatas[Index].Stamina) < 0)
		{
			Index = 0;
			bExsit = false;
			return;
		}
	}

	bExsit = false;
	Index++;

	if (Character != nullptr)
	{
		Character->UseMp(ActionDatas[Index].MP);
		Character->UseStamina(ActionDatas[Index].Stamina);
	}
	ActionDatas[Index].DoAction(Owner);
}

void UCSkills_AirCombo::End_Skill_Implementation()
{
	bTest = true;
	Super::End_Skill_Implementation();

	Attachment->OnAttachmentEndCollision.AddDynamic(DoAction, &UCDoAction::OnAttachmentEndCollision);
	Attachment->OnAttachmentBeginOverlap.AddDynamic(DoAction, &UCDoAction::OnAttachmentBeginOverlap);

	State->SetIdleMode();
	State->OffSkillMode();
	Hitted.Empty();
	Index = 1;
}

void UCSkills_AirCombo::Tick_Implementation(float InDeltaTime)
{
	Super::Tick_Implementation(InDeltaTime);
	if (Owner->GetCharacterMovement()->IsFalling() == false && bTest == true)
	{
		Land();
	}
}

void UCSkills_AirCombo::DrawCollision()
{
	ACPlayer* player = Cast<ACPlayer>(Owner);
	if (player == nullptr)
		return;
	if(bDebug == true)
		DrawDebugSphere(Owner->GetWorld(), player->GetAirArrowComponent()->GetComponentLocation(), HitSphereRadius, 12, FColor::Red, false, LifeTime, 0, 0);
	TArray<FOverlapResult> result{};
	FCollisionQueryParams params;
	params.AddIgnoredActor(Owner);
	bool hitResult = Owner->GetWorld()->OverlapMultiByObjectType(result, player->GetAirArrowComponent()->GetComponentLocation(), FQuat::Identity, ECC_Pawn, FCollisionShape::MakeSphere(HitSphereRadius), params);
	if (hitResult == true)
		HitActor(result);
	
}

void UCSkills_AirCombo::HitActor(TArray<FOverlapResult> Results)
{
	HitIndex = Index;

	if(HitIndex == 0)
	{
		ACPlayer* p = Cast<ACPlayer>(Owner);
		p->GetAirArrowComponent()->SetRelativeLocation(FVector(300, 0, 0));
	}
	for (FOverlapResult hit : Results)
	{
		ACharacter* character = Cast<ACharacter>(hit.GetActor());
		if (character != nullptr)
		{
			Hitted.AddUnique( character);

			HitDatas[HitIndex].SendDamage(Owner, Attachment, character);
		}
	}
}


void UCSkills_AirCombo::Land()
{
	bTest = false;
	Index = 0;
}

