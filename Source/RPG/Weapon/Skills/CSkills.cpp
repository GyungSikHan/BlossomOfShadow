#include "CSkills.h"
#include "GameFramework/Character.h"
#include "RPG/Components/CStateComponent.h"
#include "RPG/Components/CMovementComponent.h"

UCSkills::UCSkills()
{
}

void UCSkills::BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment, UCDoAction* InDoAction)
{
	Character = Cast<IIMPStamina>(InOwner);
	Owner = InOwner;
	Attachment = InAttachment;
	DoAction = InDoAction;

	State = Cast<UCStateComponent>(Owner->GetComponentByClass(UCStateComponent::StaticClass()));
	Movement = Cast<UCMovementComponent>(Owner->GetComponentByClass(UCMovementComponent::StaticClass()));
}

void UCSkills::Skill_Pressed()
{
	bInAction = true;
}

void UCSkills::Skill_Released()
{
	bInAction = false;
}

