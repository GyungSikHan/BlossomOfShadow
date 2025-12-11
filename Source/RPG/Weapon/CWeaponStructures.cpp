#include "CWeaponStructures.h"
#include "RPG/Utilities/CLog.h"
#include "NiagaraCommon.h"
#include "Kismet/GameplayStatics.h"
#include "RPG/Components/CMovementComponent.h"
#include "Particles/ParticleSystem.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "TimerManager.h"
#include "Animation/AnimInstance.h"
#include "Characters/CCharacter.h"
#include "Components/CStateComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "Engine/DamageEvents.h"
#include "Engine/Level.h"
#include "GameFramework/CharacterMovementComponent.h"

void FDoActionData::DoAction(ACharacter* InOwner)
{
	UCMovementComponent* movement = Cast<UCMovementComponent>(InOwner->GetComponentByClass(UCMovementComponent::StaticClass()));

	if(movement != nullptr)
	{
		if (bFixedCamera)
			movement->EnableFixedCamera();

		if (bCanMove == false)
			movement->Stop();
	}

	UCStateComponent* state = Cast<UCStateComponent>(InOwner->GetComponentByClass(UCStateComponent::StaticClass()));
	if (state != nullptr)
		state->SetInterruptible(bInterruptible);
	
	if (Montage != nullptr)
		InOwner->PlayAnimMontage(Montage, PlayRate);
}

void FDoActionData::StopAction(ACharacter* InOwner)
{
	UCMovementComponent* movement = Cast<UCMovementComponent>(InOwner->GetComponentByClass(UCMovementComponent::StaticClass()));

	if (movement != nullptr)
	{
		if (bFixedCamera)
			movement->EnableFixedCamera();

		if (bCanMove == false)
			movement->Stop();
	}

	UCStateComponent* state = Cast<UCStateComponent>(InOwner->GetComponentByClass(UCStateComponent::StaticClass()));
	if (state != nullptr)
		state->SetInterruptible(bInterruptible);

	if (Montage != nullptr)
		InOwner->GetMesh()->GetAnimInstance()->Montage_Stop(0.2f, Montage);
}

//void FDoActionData::PlaySound(ACharacter* InOwner)
//{
//	if (Sound == nullptr)
//		return;
//
//	UWorld* world = InOwner->GetWorld();
//	FVector location = InOwner->GetActorLocation();
//
//	UGameplayStatics::SpawnSoundAtLocation(world, Sound, location);
//}

void FDoActionData::PlayEffect(UWorld* InWorld, const FVector& InLocation)
{
	if (Effect == nullptr)
		return;

	FTransform transform;

	transform.SetLocation(EffectLocation);
	transform.SetScale3D(EffectScale);
	transform.AddToTranslation(InLocation);

	UParticleSystem* particle = Cast<UParticleSystem>(Effect);
	UNiagaraSystem* niagara = Cast<UNiagaraSystem>(Effect);
	
	FVector location = transform.GetLocation();
	FRotator rotator = FRotator(transform.GetRotation());
	FVector scale = transform.GetScale3D();
	FName soketName = NAME_None;

	if (!!Effect)
	{
		if (!!particle)
		{
			UGameplayStatics::SpawnEmitterAttached(particle,nullptr,soketName,location,rotator,scale);
			return;
		}
	
		if (!!niagara)
		{
			UNiagaraFunctionLibrary::SpawnSystemAttached(niagara, nullptr, soketName, location, rotator, scale,EAttachLocation::KeepRelativeOffset,true,ENCPoolMethod::None);

			return;
		}
	}
	
	if (!!particle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(InWorld, particle, transform);
		return;
	}
	
	if (!!niagara)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(InWorld, niagara, location, rotator, scale);
	}
}

void FDoActionData::PlayEffect(UWorld* InWorld, const FVector& InLocation, const FRotator& InRotation)
{
	if (Effect == nullptr)
		return;

	FTransform transform;

	transform.SetLocation(InLocation + InRotation.RotateVector(EffectLocation));
	transform.SetScale3D(EffectScale);

	UParticleSystem* particle = Cast<UParticleSystem>(Effect);
	UNiagaraSystem* niagara = Cast<UNiagaraSystem>(Effect);

	FVector location = transform.GetLocation();
	FRotator rotator = FRotator(transform.GetRotation());
	FVector scale = transform.GetScale3D();
	FName soketName = NAME_None;

	if (!!Effect)
	{
		if (!!particle)
		{
			UGameplayStatics::SpawnEmitterAttached(particle, nullptr, soketName, location, rotator, scale);
			return;
		}

		if (!!niagara)
		{
			UNiagaraFunctionLibrary::SpawnSystemAttached(niagara, nullptr, soketName, location, rotator, scale, EAttachLocation::KeepRelativeOffset, true, ENCPoolMethod::None);

			return;
		}
	}

	if (!!particle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(InWorld, particle, transform);
		return;
	}

	if (!!niagara)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(InWorld, niagara, location, rotator, scale);
	}
}


///////////////////////////////////////////////////////////////////////////////

void FHitData::SendDamage(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther)
{
	FActionDamageEvent e;
	e.HitData = this;
	InOther->TakeDamage(Power, e, InAttacker->GetController(), InAttacker);
}

void FHitData::PlayMontage(ACharacter* InOwner)
{
	if (!!Montage)
		InOwner->PlayAnimMontage(Montage, PlayRate);
}

void FHitData::PlayHitStop(UWorld* InWorld)
{
	if (FMath::IsNearlyZero(StopTime) == true)
		return;

	TArray<ACharacter*> characters;

	for (AActor* actor : InWorld->GetCurrentLevel()->Actors)
	{
		ACharacter* character = Cast<ACharacter>(actor);

		if (!!character)
		{
			character->CustomTimeDilation = 1e-3f;
			characters.Add(character);
		}
	}

	FTimerDelegate timerDelegate;
	timerDelegate.BindLambda([=]
		{
			for (ACharacter* character : characters)
				character->CustomTimeDilation = 1;
		});

	FTimerHandle timerHandle;
	InWorld->GetTimerManager().SetTimer(timerHandle, timerDelegate, StopTime, false);
}

void FHitData::PlaySound(ACharacter* InOwner)
{
	if (Sound == nullptr)
		return;

	UWorld* world = InOwner->GetWorld();
	FVector location = InOwner->GetActorLocation();

	UGameplayStatics::SpawnSoundAtLocation(world, Sound, location);
}

void FHitData::PlayEffect(UWorld* InWorld, const FVector& InLocation)
{
	if(Effect == nullptr)
		return;

	FTransform transform;

	transform.SetLocation(EffectLocation);
	transform.SetScale3D(EffectScale);
	transform.AddToTranslation(InLocation);

	UParticleSystem* particle = Cast<UParticleSystem>(Effect);
	UNiagaraSystem* niagara = Cast<UNiagaraSystem>(Effect);

	FVector location = transform.GetLocation();
	FRotator rotator = FRotator(transform.GetRotation());
	FVector scale = transform.GetScale3D();
	FName soketName = NAME_None;
	USkeletalMeshComponent* mesh{};


	if (!!Effect)
	{
		if (!!particle)
		{
			UGameplayStatics::SpawnEmitterAttached(particle, mesh, soketName, location, rotator, scale);
			return;
		}

		if (!!niagara)
		{
			UNiagaraFunctionLibrary::SpawnSystemAttached(niagara, mesh, soketName, location, rotator, scale, EAttachLocation::KeepRelativeOffset, true, ENCPoolMethod::None);

			return;
		}
	}

	if (!!particle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(InWorld, particle, transform);
		return;
	}

	if (!!niagara)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(InWorld, niagara, location, rotator, scale);
	}
}

void FHitData::PlayEffect(UWorld* InWorld, const FVector& InLocation, const FRotator& InRotation)
{
	if(Effect == nullptr)
		return;

	FTransform transform;
	transform.SetLocation(InLocation + InRotation.RotateVector(EffectLocation));
	transform.SetScale3D(EffectScale);

	UParticleSystem* particle = Cast<UParticleSystem>(Effect);
	UNiagaraSystem* niagara = Cast<UNiagaraSystem>(Effect);

	FVector location = transform.GetLocation();
	FRotator rotation = FRotator(transform.GetRotation());
	FVector scale = transform.GetScale3D();

	USkeletalMeshComponent* mesh{};
	FName name = NAME_None;

	if (mesh != nullptr)
	{
		if (!!particle)
		{
			UGameplayStatics::SpawnEmitterAttached(particle, mesh, name, location, rotation, scale);

			return;
		}

		if (!!niagara)
		{
			UNiagaraFunctionLibrary::SpawnSystemAttached(niagara, mesh, name, location, rotation, scale, EAttachLocation::KeepRelativeOffset, true, ENCPoolMethod::None);

			return;
		}
	}

	if (!!particle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(InWorld, particle, transform);

		return;
	}

	if (!!niagara)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(InWorld, niagara, location, rotation, scale);

		return;
	}
}

///////////////////////////////////////////////////////////////////////////////

void FSubAction::DoSubAction(ACharacter* InOwner)
{
	UCMovementComponent* movement = Cast<UCMovementComponent>(InOwner->GetComponentByClass(UCMovementComponent::StaticClass()));

	if (!!movement)
	{
		if (bFixedCamera)
			movement->EnableFixedCamera();

		if (bCanMove == false)
			movement->Stop();
	}

	if (!!Montage)
		InOwner->PlayAnimMontage(Montage, PlayRate);
}
