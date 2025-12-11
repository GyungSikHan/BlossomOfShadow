#include "CAnimNotify_CameraShake.h"
#include "GameFramework/Character.h"

FString UCAnimNotify_CameraShake::GetNotifyName_Implementation() const
{
	return "CameraShake";
}

void UCAnimNotify_CameraShake::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp == nullptr)
		return;
	if (MeshComp->GetOwner() == nullptr)
		return;

	ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner());
	if(Character == nullptr)
		return;

	APlayerController* controller = Character->GetController<APlayerController>();
	if(controller == nullptr)
		return;

	controller->PlayerCameraManager->StartCameraShake(CameraShakeClass);
}
