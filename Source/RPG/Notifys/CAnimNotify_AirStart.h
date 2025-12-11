// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CAnimNotify_AirStart.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UCAnimNotify_AirStart : public UAnimNotify
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EDrawDebugTrace::Type> Debug;
	UPROPERTY(EditAnywhere)
	FVector Launch;
private:
	virtual FString GetNotifyName_Implementation() const;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

};
