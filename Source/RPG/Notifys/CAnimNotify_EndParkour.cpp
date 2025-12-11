// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifys/CAnimNotify_EndParkour.h"
#include "Components/CParkourComponent.h"
#include "GameFramework/Character.h"
#include "Weapon/CDoAction.h"

FString UCAnimNotify_EndParkour::GetNotifyName_Implementation() const
{
	return "Parkour";
}

void UCAnimNotify_EndParkour::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (MeshComp == nullptr)
		return;
	if(MeshComp->GetOwner() == nullptr)
		return;

	UCParkourComponent* parkour = Cast<UCParkourComponent>(MeshComp->GetOwner()->GetComponentByClass(UCParkourComponent::StaticClass()));
	if(parkour == nullptr)
		return;
	parkour->End_DoParkour();

}
