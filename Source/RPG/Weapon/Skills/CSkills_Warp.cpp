#include "CSkills_Warp.h"
#include "RPG/Utilities/CLog.h"
#include "Components/DecalComponent.h"
#include "GameFramework/Character.h"
#include "RPG/Components/CAIBehaviorComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "RPG/Weapon/CAttachment.h"
#include "RPG/Weapon/Add_On/CWarp.h"

UCSkills_Warp::UCSkills_Warp()
{
}

void UCSkills_Warp::BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment, UCDoAction* InDoAction)
{
	Super::BeginPlay(InOwner, InAttachment, InDoAction);

	Decal = Cast<UDecalComponent>(InAttachment->GetComponentByClass(UDecalComponent::StaticClass()));
	PlayerController = Owner->GetController<APlayerController>();
	Behavior = Cast<UCAIBehaviorComponent>(InOwner->GetComponentByClass(UCAIBehaviorComponent::StaticClass()));

}

void UCSkills_Warp::Tick_Implementation(float InDeltaTime)
{
	Super::Tick_Implementation(InDeltaTime);

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

void UCSkills_Warp::Skill_Pressed()
{
	if (Character != nullptr)
	{
		if ((Character->GetMp() - ActionDatas[Index].MP) < 0 || (Character->GetStamina() - ActionDatas[Index].Stamina) < 0)
			return;
	}
	//if (State->IsIdleMode() == false)
	//	return;
	//if (State->IsSkillMode() == true)
	//	return;
	Super::Skill_Pressed();
	
	Decal->SetVisibility(true);
}

void UCSkills_Warp::Skill_Released()
{
	Super::Skill_Released();
	Decal->SetVisibility(false);
}

bool UCSkills_Warp::GetCursorLocationAndRotation(FVector& OutLocation, FRotator& OutRotation)
{
	if(PlayerController == nullptr)
		return false;


	FHitResult hitResult;
	PlayerController->GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1, false, hitResult);
	if (hitResult.bBlockingHit == false)
		return false;


	OutLocation = hitResult.Location;
	OutRotation = hitResult.ImpactNormal.Rotation();

	return true;
}
