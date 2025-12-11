#include "AnimNotifyState_Particle.h"
#include "NiagaraFunctionLibrary.h"
#include "RPG/Components/CWeaponComponent.h"
#include "Kismet/GameplayStatics.h"
#include "RPG/Weapon/CWeaponStructures.h"
#include "RPG/Weapon/Skills/CSkills.h"

FString UAnimNotifyState_Particle::GetNotifyName_Implementation() const
{
	return "PlayParticle";
}

void UAnimNotifyState_Particle::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if(MeshComp == nullptr)
		return;
	if (MeshComp->GetOwner() == nullptr)
		return;

	UCWeaponComponent* weapon = Cast<UCWeaponComponent>(MeshComp->GetOwner()->GetComponentByClass(UCWeaponComponent::StaticClass()));
	if (weapon == nullptr)
		return;
	//FDoActionData action = weapon->GetSkills()->ActionDatas;
	for (auto action : weapon->GetSkills()->ActionDatas)
	{
		
		FTransform transform;
		transform.SetLocation(MeshComp->GetOwner()->GetActorLocation() + MeshComp->GetOwner()->GetActorRotation().RotateVector(action.EffectLocation));
		transform.SetScale3D(action.EffectScale);
		location = transform.GetLocation();
		rotator = FRotator(transform.GetRotation());
		scale = transform.GetScale3D();
		Niagara = Cast<UNiagaraSystem>(action.Effect);
		Particle = Cast<UParticleSystem>(action.Effect);
	
		if (!!Particle)
		{
			ParticleComponent = UGameplayStatics::SpawnEmitterAtLocation(MeshComp->GetOwner()->GetWorld(), Particle, transform);
			return;
		}

		if (!!Niagara)
		{
			NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(MeshComp->GetOwner()->GetWorld(), Niagara, location, rotator, scale);
		}
	}

}

void UAnimNotifyState_Particle::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (MeshComp == nullptr)
		return;
	if (MeshComp->GetOwner() == nullptr)
		return;

	UFXSystemComponent* com = Cast<UFXSystemComponent>(NiagaraComponent);

	if(com == nullptr)
		com = Cast<UFXSystemComponent>(ParticleComponent);

	if (com == nullptr)
		return;

	com->Deactivate();
}