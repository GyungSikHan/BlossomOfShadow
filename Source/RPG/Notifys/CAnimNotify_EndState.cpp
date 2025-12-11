#include "CAnimNotify_EndState.h"
#include "RPG/Characters/Interface/ICharacter.h"

FString UCAnimNotify_EndState::GetNotifyName_Implementation() const
{
	return "EndState";
}

void UCAnimNotify_EndState::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (MeshComp == nullptr)
		return;
	if (MeshComp->GetOwner() == nullptr)
		return;

	IICharacter* character = Cast<IICharacter>(MeshComp->GetOwner());

	if (character == nullptr)
		return;

	switch (StateType)
	{
	case EStateType::BackStep:
		character->End_BackStep();
		break;
	case EStateType::Roll_F:
	case EStateType::Roll_B:
	case EStateType::Roll_L:
	case EStateType::Roll_R:
	case EStateType::Roll_FR:
	case EStateType::Roll_FL:
	case EStateType::Roll_BR:
	case EStateType::Roll_BL:
		character->End_Roll();
		break;
	case EStateType::Hitted:
		character->End_Hitted();
		break;
	case EStateType::Dead:
		character->End_Dead();
		break;
	default:
		break;
	}
}
