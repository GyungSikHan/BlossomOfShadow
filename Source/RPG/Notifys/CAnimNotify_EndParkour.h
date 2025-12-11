// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_EndParkour.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UCAnimNotify_EndParkour : public UAnimNotify
{
	GENERATED_BODY()
private:
	virtual FString GetNotifyName_Implementation() const;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
