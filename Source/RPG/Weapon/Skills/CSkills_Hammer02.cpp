#include "CSkills_Hammer02.h"
#include "Components/CapsuleComponent.h"
#include "RPG/Components/CStateComponent.h"
#include "RPG/Components/CTargetComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "RPG/Weapon/Add_On/CAura.h"
#include "RPG/Characters/AI/CAIController.h"
#include "RPG/Components/CAIBehaviorComponent.h"

void UCSkills_Hammer02::Skill_Pressed()
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

	Target = Cast<UCTargetComponent>(Owner->GetComponentByClass(UCTargetComponent::StaticClass()));

	State->SetActionMode();
	State->OnSkillMode();
	
	bMoving = true;
	if(Target != nullptr && Target->GetTargetting() == false)
		Target->Toggle_Target();

	if (Character != nullptr)
	{
		Character->UseMp(ActionDatas[Index].MP);
		Character->UseStamina(ActionDatas[Index].Stamina);
	}

	ActionDatas[Index].DoAction(Owner);

	Start = Owner->GetActorLocation();
	End = Start + Owner->GetActorForwardVector() * TraceDistance;
	ACharacter* target;
	if(Target != nullptr)
		target = Target->GetTarget();
	else
	{
		ACEnemy_AI* ai = Cast<ACEnemy_AI>(Owner);
		UCAIBehaviorComponent* behavior = Cast<UCAIBehaviorComponent>(ai->GetComponentByClass(UCAIBehaviorComponent::StaticClass()));
		target = behavior->GetTarget();
	}
	if(target != nullptr)
		End = target->GetActorLocation();
	
	if (DrawDebug == EDrawDebugTrace::ForDuration)
		DrawDebugDirectionalArrow(Owner->GetWorld(), Start, End, 25, FColor::Black, true, 5, 0, 0);
}

void UCSkills_Hammer02::Begin_Skill_Implementation()
{
	Super::Begin_Skill_Implementation();
	if (bMoving == false)
		return;


	FVector location = Owner->GetActorLocation();
	float radius = Owner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	if (location.Equals(End, radius))
	{
		bMoving = false;
		Start = End = Owner->GetActorLocation();
		return;
	}

	FVector direction = (End - Start).GetSafeNormal2D();
	Owner->AddActorWorldOffset(direction * Speed, true);
}

void UCSkills_Hammer02::End_Skill_Implementation()
{
	Super::End_Skill_Implementation();

	FActorSpawnParameters params;
	params.Owner = Cast<ACharacter>(Owner);
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FTransform transform;
	transform.SetLocation(Owner->GetActorLocation());
	transform.AddToTranslation(AuraLocation);
	transform.SetRotation(FQuat(Owner->GetActorRotation()));

	Owner->GetWorld()->SpawnActor<ACAura>(AuraClass, transform, params);

	if(Target != nullptr)
		Target->EndTarget();

	State->SetIdleMode();
	State->OffSkillMode();
}
