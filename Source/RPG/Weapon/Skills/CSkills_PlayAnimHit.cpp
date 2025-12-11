#include "CSkills_PlayAnimHit.h"
#include "RPG/Components/CMovementComponent.h"
#include "RPG/Components/CStateComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "RPG/Weapon/CAttachment.h"
#include "RPG/Weapon/CDoAction.h"

void UCSkills_PlayAnimHit::Skill_Pressed()
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

void UCSkills_PlayAnimHit::Begin_Skill_Implementation()
{
	Super::Begin_Skill_Implementation();
	Attachment->OnAttachmentEndCollision.Remove(DoAction, "OnAttachmentEndCollision");
	Attachment->OnAttachmentBeginOverlap.Remove(DoAction, "OnAttachmentBeginOverlap");

	Attachment->OnAttachmentEndCollision.AddDynamic(this, &UCSkills_PlayAnimHit::OnAttachmentEndCollision);
	Attachment->OnAttachmentBeginOverlap.AddDynamic(this, &UCSkills_PlayAnimHit::OnAttachmentBeginOverlap);
}

void UCSkills_PlayAnimHit::End_Skill_Implementation()
{
	Super::End_Skill_Implementation();
	
	Attachment->OnAttachmentEndCollision.Remove(this, "OnAttachmentEndCollision");
	Attachment->OnAttachmentBeginOverlap.Remove(this, "OnAttachmentBeginOverlap");

	Attachment->OnAttachmentEndCollision.AddDynamic(DoAction, &UCDoAction::OnAttachmentEndCollision);
	Attachment->OnAttachmentBeginOverlap.AddDynamic(DoAction, &UCDoAction::OnAttachmentBeginOverlap);

	State->SetIdleMode();
	State->OffSkillMode();

	HitIndex = 0;
}

void UCSkills_PlayAnimHit::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCuaser, ACharacter* InOther)
{
	if(InOther == nullptr)
		return;

	for (ACharacter* character : Hitted)
		if(character == InOther)
			return;

	Hitted.AddUnique(InOther);

	if(HitIndex >= HitDatas.Num())
		return;

	if (isTarget == true)
	{
		float angle = -2.0f;
		ACharacter* candidate = nullptr;

		for (ACharacter* hitted : Hitted)
		{
			FVector direction = hitted->GetActorLocation() - Owner->GetActorLocation();
			direction = direction.GetSafeNormal2D();

			FVector forward = FQuat(Owner->GetActorRotation()).GetForwardVector();

			float dot = FVector::DotProduct(direction, forward);
			if (dot >= angle)
			{
				angle = dot;
				candidate = hitted;
			}
		}

		if (candidate != nullptr)
		{
			FRotator rotation = UKismetMathLibrary::FindLookAtRotation(Owner->GetActorLocation(), candidate->GetActorLocation());
			FRotator target = FRotator(0, rotation.Yaw, 0);

			AController* controller = Owner->GetController<AController>();
			controller->SetControlRotation(target);
		}
	}

	HitDatas[HitIndex].SendDamage(Owner, InAttackCuaser, InOther);
}

void UCSkills_PlayAnimHit::OnAttachmentEndCollision()
{
	Hitted.Empty();
	HitIndex++;
}
