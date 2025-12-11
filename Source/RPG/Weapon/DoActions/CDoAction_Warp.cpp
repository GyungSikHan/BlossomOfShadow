#include "CDoAction_Warp.h"
#include "RPG/Weapon/CAttachment.h"
#include "Components/CapsuleComponent.h"
#include "Components/DecalComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "RPG/Components/CAIBehaviorComponent.h"
#include "RPG/Components/CStateComponent.h"

UCDoAction_Warp::UCDoAction_Warp()
{
}

void UCDoAction_Warp::BeginPlay(ACAttachment* InAttachment, UCEquipment* InEquipment, ACharacter* InOwner,
	TArray<FDoActionData>& InDoActionDatas, const TArray<FHitData>& InHitData)
{
	Super::BeginPlay(InAttachment, InEquipment, InOwner, InDoActionDatas, InHitData);
	Decal = Cast<UDecalComponent>(InAttachment->GetComponentByClass(UDecalComponent::StaticClass()));
	PlayerController = OwnerCharacter->GetController<APlayerController>();

	Behavior = Cast<UCAIBehaviorComponent>(InOwner->GetComponentByClass(UCAIBehaviorComponent::StaticClass()));
}

void UCDoAction_Warp::Tick(float InDeltaTime)
{
	Super::Tick(InDeltaTime);

	FVector location = FVector::ZeroVector;
	FRotator rotation = FRotator::ZeroRotator;

	if (GetCursorLocationAndRotation(location, rotation) == false)
	{
		Decal->SetVisibility(false);

		return;
	}

	if (bInAction)
		return;


	Decal->SetVisibility(true);

	Decal->SetWorldLocation(location);
	Decal->SetWorldRotation(rotation);
}

void UCDoAction_Warp::DoAction()
{
	if(DoActionDatas.Num() <= 0)
		return;
	if (character != nullptr && Index == 0)
	{
		if ((character->GetMp() - DoActionDatas[Index].MP) < 0 || (character->GetStamina() - DoActionDatas[Index].Stamina) < 0)
			return;
	}
	if(State->IsIdleMode() == false)
		return;

	Super::DoAction();

	FRotator rotation;
	if (GetCursorLocationAndRotation(MoveToLocation, rotation))
	{
		float height = OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		MoveToLocation = FVector(MoveToLocation.X, MoveToLocation.Y, MoveToLocation.Z + height);

		float yaw = UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), MoveToLocation).Yaw;
		OwnerCharacter->SetActorRotation(FRotator(0, yaw, 0));
	}

	if (character != nullptr)
	{
		character->UseMp(DoActionDatas[Index].MP);
		character->UseStamina(DoActionDatas[Index].Stamina);
	}
	DoActionDatas[0].DoAction(OwnerCharacter);
}

void UCDoAction_Warp::Begin_DoAction()
{
	Super::Begin_DoAction();

	if (!!PlayerController)
	{
		OwnerCharacter->SetActorLocation(MoveToLocation);
		MoveToLocation = FVector::ZeroVector;

		return;
	}

	if(Behavior == nullptr)
		return;
	OwnerCharacter->SetActorLocation(Behavior->GetAvoidlocation());
}

bool UCDoAction_Warp::GetCursorLocationAndRotation(FVector& OutLocation, FRotator& OutRotation)
{
	if(PlayerController == nullptr)
		return false;

	FHitResult hitResult;
	PlayerController->GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1, false, hitResult);
	if (hitResult.bBlockingHit == false)
		return 	false;

	OutLocation = hitResult.Location;
	OutRotation = hitResult.ImpactNormal.Rotation();

	return true;
}
