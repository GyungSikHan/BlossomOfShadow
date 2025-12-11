#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_Particle.generated.h"

UCLASS()
class RPG_API UAnimNotifyState_Particle : public UAnimNotifyState
{
	GENERATED_BODY()
private:
	class UNiagaraSystem* Niagara;
	FVector location;
	FRotator rotator;
	FVector scale;
	FName soketName;
	UNiagaraComponent* NiagaraComponent;
	UParticleSystemComponent* ParticleComponent;
	UParticleSystem* Particle;

private:
	FString GetNotifyName_Implementation() const;
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference);
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference);


};

 