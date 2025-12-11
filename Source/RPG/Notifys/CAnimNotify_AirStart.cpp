// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifys/CAnimNotify_AirStart.h"

#include "GameFramework/Character.h"

FString UCAnimNotify_AirStart::GetNotifyName_Implementation() const
{
	return "Air Combo Start Launch";
}

void UCAnimNotify_AirStart::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (MeshComp == nullptr)
		return;
	ACharacter* character = Cast<ACharacter>(MeshComp->GetOwner());
	if (character == nullptr)
		return;

	FVector start = character->GetActorLocation();
	FVector end = FVector(start.X, start.Y, start.Z - 1200.0f);
	TArray<TEnumAsByte<EObjectTypeQuery>> types;
	types.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	types.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	TArray<AActor*> ignores;
	ignores.Add(character);
	TArray<FHitResult> hit;
	bool result = UKismetSystemLibrary::LineTraceMultiForObjects(character->GetWorld(), start, end, types, false, ignores, Debug, hit, true);
	if(result == true)
		character->LaunchCharacter(Launch, true, true);
}
