#include "CTargetComponent.h"
#include "CStateComponent.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "RPG/Utilities/CBF_NearlyAngle.h"

UCTargetComponent::UCTargetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCTargetComponent::BeginPlay()
{
	Super::BeginPlay();
	Character = Cast<ACharacter>(GetOwner());
	
}

void UCTargetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (Target == nullptr)
		return;

	UCStateComponent* state = Cast<UCStateComponent>(Target->GetComponentByClass(UCStateComponent::StaticClass()));

	if(state->IsDeadMode() == true)
	{
		EndTarget();
		return;
	}
	if (Character->GetDistanceTo(Target) <= TraceDistance)
		TickTarget();
	else
		EndTarget();
}

void UCTargetComponent::Toggle_Target()
{
	if (Target == nullptr)
	{
		BeginTarget();
		return;
	}

	EndTarget();
}

void UCTargetComponent::ChangeFocus(const FInputActionValue& Value)
{
	if (Target == nullptr)
		return;

	TMap<float, ACharacter*> map;
	TArray<AActor*> actors;
	TArray<ACharacter*> targets;
	FVector start = Character->GetActorLocation();
	FVector end = start;

	actors.Add(Character);

	TArray<FHitResult> hitResult;
	UKismetSystemLibrary::SphereTraceMultiByProfile(GetWorld(), start, end, TraceDistance, "Targetting", false, actors, DrawDebug, hitResult, true);

	for (FHitResult hit : hitResult)
	{
		ACharacter* target = Cast<ACharacter>(hit.GetActor());
		FVector targetLocation = target->GetActorLocation();
		FVector characterLocation = Character->GetActorLocation();
		FVector normal = UKismetMathLibrary::Vector_Normal2D(targetLocation - characterLocation);
		FVector characterForward = UKismetMathLibrary::GetForwardVector(Character->GetControlRotation());
		float key = UKismetMathLibrary::Dot_VectorVector(UKismetMathLibrary::Cross_VectorVector(characterForward, normal), Character->GetActorUpVector());
		map.Add(key, target);
	}

	TArray<float> keyArrays;
	map.GetKeys(keyArrays);

	ACharacter* candidate{};
	float key{};
	double minimum = 999999.0;

	for (double keyArray : keyArrays)
	{
		key = keyArray;

		if (Value.Get<float>() == 1.0f)
		{
			if(key > 0.0)
			{
				if(UKismetMathLibrary::Abs(key) < minimum)
				{
					minimum = UKismetMathLibrary::Abs(key);
					candidate = *map.Find(key);
				}
			}
		}
		else
		{
			if(key < 0.0f)
			{
				if(UKismetMathLibrary::Abs(key) < minimum)
				{
					minimum = UKismetMathLibrary::Abs(key);
					candidate = *map.Find(key);
				}
			}
		}
	}

	if (candidate != nullptr)
		if (bMovingFocus == false)
			bMovingFocus = true;

	ChangeTarget(candidate);
}

void UCTargetComponent::EndTarget()
{
	Target = nullptr;
	if (Particle != nullptr)
		Particle->DestroyComponent();
	bTargetting = false;
}

void UCTargetComponent::BeginTarget()
{
	TArray<AActor*> actors;
	TArray<ACharacter*> targets;
	FVector start = Character->GetActorLocation();
	FVector end = start;

	actors.Add(Character);

	TArray<FHitResult> hitResults;
	UKismetSystemLibrary::SphereTraceMultiByProfile(GetWorld(), start, end, TraceDistance, "Targetting", false, actors, DrawDebug, hitResults, true);

	for (FHitResult hit : hitResults)
		if (hit.GetActor()->GetClass() != Character->GetClass())
			targets.Add(Cast<ACharacter>(hit.GetActor()));

	UCBF_NearlyAngle* helpers{};
	ACharacter* target = helpers->GetNearlyFrontAngle(Character, targets);
	ChangeTarget(target);
}

void UCTargetComponent::TickTarget()
{
	FRotator ControlRotation = Character->GetControlRotation();
	FRotator find = UKismetMathLibrary::FindLookAtRotation(Character->GetActorLocation(), Target->GetActorLocation());
	FRotator ownerToTarget = FRotator(ControlRotation.Pitch, find.Yaw, find.Roll);

	if (UKismetMathLibrary::EqualEqual_RotatorRotator(ControlRotation, ownerToTarget, FinishAngle))
	{
		Character->GetController()->SetControlRotation(ownerToTarget);
		if (bMovingFocus == true)
			bMovingFocus = false;
	}
	else
		Character->GetController()->SetControlRotation(UKismetMathLibrary::RInterpTo(ControlRotation, UKismetMathLibrary::MakeRotator(ownerToTarget.Roll, ownerToTarget.Pitch, ownerToTarget.Yaw), GetWorld()->GetDeltaSeconds(), InterpSpeed));
}

void UCTargetComponent::ChangeTarget(ACharacter* InCandidate)
{
	if(InCandidate == nullptr)
	{
		EndTarget();
		return;
	}

	bTargetting = true;

	if (Particle != nullptr)
		Particle->DestroyComponent();

	Particle = UGameplayStatics::SpawnEmitterAttached(ParticleAsset, InCandidate->GetMesh(), "Root", ParticleLocation, FRotator(0, 0, 0), ParticleScael);
	Target = InCandidate;
}

