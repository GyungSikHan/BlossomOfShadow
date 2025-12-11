#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_EndSkill.generated.h"

UCLASS()
class RPG_API UCAnimNotify_EndSkill : public UAnimNotify
{
	GENERATED_BODY()
private:
	virtual FString GetNotifyName_Implementation() const;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
