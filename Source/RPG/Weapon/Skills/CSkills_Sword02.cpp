#include "CSkills_Sword02.h"
#include "Components/CapsuleComponent.h"
#include "RPG/Components/CStateComponent.h"
#include "GameFramework/Character.h"
#include "RPG/Weapon/CAttachment.h"
#include "RPG/Weapon/CDoAction.h"

void UCSkills_Sword02::Skill_Pressed()
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

void UCSkills_Sword02::Begin_Skill_Implementation()
{
	Super::Begin_Skill_Implementation();
	Attachment->OnAttachmentBeginOverlap.Remove(DoAction, "OnAttachmentBeginOverlap");
	Attachment->OnAttachmentBeginOverlap.AddDynamic(this, &UCSkills_Sword02::OnAttachmentBeginOverlap);

	bMoving = true;

	Start = Owner->GetActorLocation();
	End = Start + Owner->GetActorForwardVector() * Distance;

	float radius = Owner->GetCapsuleComponent()->GetScaledCapsuleRadius();
	float height = Owner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	FRotator rotation = Owner->GetActorRotation();

	TArray<AActor*> ignores;
	ignores.Add(Owner);

	TArray<FHitResult> hitResults;
	TArray<TEnumAsByte<EObjectTypeQuery>> objects;
	objects.Add(EObjectTypeQuery::ObjectTypeQuery3);
	objects.Add(EObjectTypeQuery::ObjectTypeQuery4);

	UKismetSystemLibrary::BoxTraceMultiForObjects(Owner->GetWorld(), Start, End, FVector(0, radius*2, height), rotation, objects, false, ignores, DrawDebug, hitResults, true);

	for (const FHitResult& hitResult : hitResults)
	{
		ACharacter* character = Cast<ACharacter>(hitResult.GetActor());

		if (!!character)
		{
			character->GetCapsuleComponent()->SetCollisionProfileName("Sword_Skill");

			Overlapped.Add(character);
		}
	}

	FHitResult lineHitResult;
	UKismetSystemLibrary::LineTraceSingle(Owner->GetWorld(), Start, End, ETraceTypeQuery::TraceTypeQuery1, false, ignores, DrawDebug, lineHitResult, true);

	if (lineHitResult.bBlockingHit)
	{
		FVector direction = (End - Start).GetSafeNormal2D();
		End = lineHitResult.Location - (direction * radius * 2);

		if (DrawDebug == EDrawDebugTrace::ForDuration)
			DrawDebugSphere(Owner->GetWorld(), End, radius * 2, 20, FColor::Black, true, 2);
	}

	if (DrawDebug == EDrawDebugTrace::ForDuration)
		DrawDebugDirectionalArrow(Owner->GetWorld(), Start, End, 25, FColor::Black, true, 5, 0, 0);
}

void UCSkills_Sword02::Tick_Implementation(float InDeltaTime)
{
	Super::Tick_Implementation(InDeltaTime);

	if(bMoving == false)
		return;

	FVector location = Owner->GetActorLocation();
	float radius = Owner->GetCapsuleComponent()->GetScaledCapsuleRadius();

	if (location.Equals(End, radius))
	{
		bMoving = false;
		Start = End = Owner->GetActorLocation();

		return;
	}

	FVector direction = (End - Start).GetSafeNormal2D();
	Owner->AddActorWorldOffset(direction * Speed, true);
}

void UCSkills_Sword02::End_Skill_Implementation()
{
	Super::End_Skill_Implementation();
	Attachment->OnAttachmentBeginOverlap.Remove(this, "OnAttachmentBeginOverlap");
	Attachment->OnAttachmentBeginOverlap.AddDynamic(DoAction, &UCDoAction::OnAttachmentBeginOverlap);

	bMoving = false;

	for (ACharacter* character : Overlapped)
		character->GetCapsuleComponent()->SetCollisionProfileName("Pawn");

	Overlapped.Empty();
	Hitted.Empty();

	State->SetIdleMode();
	State->OffSkillMode();
}

void UCSkills_Sword02::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCuaser, ACharacter* InOther)
{
	if(InOther == nullptr)
		return;

	for (ACharacter* character : Hitted)
		if(character == InOther)
			return;

	Hitted.AddUnique(InOther);

	HitData.SendDamage(Owner, InAttackCuaser, InOther);
}