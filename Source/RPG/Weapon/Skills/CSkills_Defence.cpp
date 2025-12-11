// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Skills/CSkills_Defence.h"
#include "Characters/CCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/OverlapResult.h"
#include "Kismet/GameplayStatics.h"
#include "Utilities/CLog.h"
#include "Weapon/CAttachment.h"
#include "Weapon/CDoAction.h"

void UCSkills_Defence::Skill_Pressed()
{
	if (State->IsIdleMode() == false)
		return;
	if(State->IsSkillMode() == true)
		return;
	Super::Skill_Pressed();

	State->SetActionMode();
	State->OnSkillMode();

	bDefence = true;
	ActionDatas[0].DoAction(Owner);
}

void UCSkills_Defence::Begin_Skill_Implementation()
{
	Super::Begin_Skill_Implementation();
	Attachment->OnAttachmentBeginOverlap.Remove(DoAction, "OnAttachmentBeginOverlap");
	CrateCollision();
}

void UCSkills_Defence::End_Skill_Implementation()
{
	Super::End_Skill_Implementation();
	DestroyCollision();
	Attachment->OnAttachmentBeginOverlap.AddDynamic(DoAction, &UCDoAction::OnAttachmentBeginOverlap);
	ACCharacter* character = Cast<ACCharacter>(Owner);
	bDefence = false;
	State->SetIdleMode();
	State->OffSkillMode();
}

void UCSkills_Defence::Skill_Released()
{
	Super::Skill_Released();
	DestroyCollision();
	Attachment->OnAttachmentBeginOverlap.AddDynamic(DoAction, &UCDoAction::OnAttachmentBeginOverlap);

	ACCharacter* character = Cast<ACCharacter>(Owner);
	ActionDatas[0].StopAction(Owner);

	Owner->GetMesh()->GetAnimInstance()->Montage_Stop(1, ActionDatas[0].Montage);

	bDefence = false;
	State->SetIdleMode();
	State->OffSkillMode();
}

void UCSkills_Defence::ParryAttack()
{
	UGameplayStatics::SetGlobalTimeDilation(Owner->GetWorld(), 0.2f);
}

void UCSkills_Defence::ResetTime()
{
	UGameplayStatics::SetGlobalTimeDilation(Owner->GetWorld(), 1.0f);
	bParry = false;
	//Owner->GetMesh()->GetAnimInstance()->Montage_Play(ActionDatas[0].Montage, 1, EMontagePlayReturnType::MontageLength, MontagePosition);
	MontagePosition = 0.0f;
}

void UCSkills_Defence::SetParry(bool InIndex)
{
	bParry = InIndex;
}

void UCSkills_Defence::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	if (OtherActor == nullptr)
		return;
	if(OtherActor == Attachment)
		return;
	ACAttachment* otherCuasser = Cast<ACAttachment>(OtherActor);
	if(otherCuasser == nullptr)
		return;

	if (bParry == true)
		ParryAttack();

	MontagePosition = Owner->GetMesh()->GetAnimInstance()->Montage_GetPosition(ActionDatas[0].Montage);
	Owner->PlayAnimMontage(Montage);
	Hitted = otherCuasser->OwnerCharacter;
	HitData.SendDamage(Owner, Attachment, otherCuasser->OwnerCharacter);
}


void UCSkills_Defence::CrateCollision()
{
	if(Sphere != nullptr)
		return;
	Sphere = NewObject<USphereComponent>(Attachment);
	Sphere->SetupAttachment(Attachment->GetRootComponent());
	Sphere->InitSphereRadius(Radius);
	if(bDebug == true)
	{
		Sphere->SetHiddenInGame(false);
		Sphere->SetVisibility(true);
	}
	Sphere->SetCollisionProfileName(L"Defence");
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &UCSkills_Defence::OnComponentBeginOverlap);
	Sphere->RegisterComponent();
}

void UCSkills_Defence::DestroyCollision()
{
	if(Sphere == nullptr)
		return;

	Hitted = nullptr;
	Sphere->DestroyComponent();
	Sphere = nullptr;
}