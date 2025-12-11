#include "CAnimInstance.h"

#include "Components/CParkourComponent.h"
#include "Components/CStateComponent.h"
#include "RPG/Utilities/CLog.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RPG/Components/CWeaponComponent.h"
#include "RPG/Weapon/Skills/CSkills.h"
#include "Weapon/CAttachment.h"
#include "Weapon/CDoAction.h"

void UCAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());

	if (OwnerCharacter == nullptr)
		return;
	Movement = OwnerCharacter->GetCharacterMovement();
	bFalling = false;
	bShouldMove = false;

	Weapon = Cast<UCWeaponComponent>(OwnerCharacter->GetComponentByClass(UCWeaponComponent::StaticClass()));
	if (Weapon != nullptr)
		Weapon->OnWeaponTypeChanged.AddDynamic(this, &UCAnimInstance::UCAnimInstance::OnWeaponTypeChanged);
	State = Cast<UCStateComponent>(OwnerCharacter->GetComponentByClass(UCStateComponent::StaticClass()));

}

void UCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (OwnerCharacter == nullptr)
		return;
	Velocity = Movement->Velocity;
	Speed = OwnerCharacter->GetVelocity().Size2D();
	if (Speed > 3 && Movement->GetCurrentAcceleration() != FVector(0.0f, 0.0f, 0.0f))
		bShouldMove = true;
	else
		bShouldMove = false;

	FRotator rotation = OwnerCharacter->GetVelocity().ToOrientationRotator();
	FRotator rotator2 = OwnerCharacter->GetControlRotation();
	FRotator delta = UKismetMathLibrary::NormalizedDeltaRotator(rotation, rotator2);
	PrevRotation = UKismetMathLibrary::RInterpTo(PrevRotation, delta, DeltaSeconds, 25);

	Direction = PrevRotation.Yaw;
	Pitch = UKismetMathLibrary::FInterpTo(Pitch, OwnerCharacter->GetBaseAimRotation().Pitch, DeltaSeconds, 25);
	bFalling = OwnerCharacter->GetCharacterMovement()->IsFalling();

	UCFeetComponent* feet = Cast<UCFeetComponent>(OwnerCharacter->GetComponentByClass(UCFeetComponent::StaticClass()));
	UCParkourComponent* parkour = Cast<UCParkourComponent>(OwnerCharacter->GetComponentByClass(UCParkourComponent::StaticClass()));
	if (Weapon == nullptr)
	{
		CLog::Print("t");
		return;
	}
	
	if (Weapon->GetSkills() != nullptr)
	{
		//Bow
		if(Weapon->GetWeaponType() == EWeaponType::Bow)
		{
			bBow_Aiming = true;
			bBow_Aiming &= WeaponType == EWeaponType::Bow;
			bBow_Aiming &= Weapon->GetSkills()->GetInAction();
		}
	}

	bFeet = false;
	if(parkour != nullptr && feet != nullptr)
	{
		bFeet = parkour->IsExecuting() == false;
		FeetData = feet->GetData();
	}
	else if(feet != nullptr)
	{
		bFeet = true;
		FeetData = feet->GetData();
	}


}

FVector UCAnimInstance::FootTraces(FVector foot, FVector pelvis)
{
	FVector addIk = UKismetMathLibrary::MakeVector(foot.X, foot.Y, (pelvis.Z + 30));
	FVector subIk = UKismetMathLibrary::MakeVector(foot.X, foot.Y, (pelvis.Z - 50));
	TArray<AActor*> ignore;
	FHitResult result;
	bool bTrace = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), addIk, subIk, 3.0f, ETraceTypeQuery::TraceTypeQuery1, false, ignore, EDrawDebugTrace::None, result, true);
 
	FVector resultVector = UKismetMathLibrary::SelectVector(FVector(0, 0, result.ImpactPoint.Z - pelvis.Z), FVector::ZeroVector, bTrace);
	resultVector = FVector(resultVector.X, resultVector.Y, resultVector.Z + 5);
	
	return resultVector;
}

void UCAnimInstance::OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType)
{
	WeaponType = InNewType;
}
