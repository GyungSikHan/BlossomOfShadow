#include "CSkills_Bow_Zomming.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "RPG/Components/CStateComponent.h"
#include "Components/WidgetComponent.h"
#include "RPG/Weapon/Attachments/CAttachment_Bow.h"
#include "UObject/ConstructorHelpers.h"
#include "Curves/CurveVector.h"
#include "Kismet/KismetSystemLibrary.h"
#include "RPG/Widget/CUserWidget_CrossHair.h"

UCSkills_Bow_Zomming::UCSkills_Bow_Zomming():
Curve{},AimData()

{
	ConstructorHelpers::FObjectFinder<UCurveVector> CurveAsset(L"/Script/Engine.CurveVector'/Game/Weapon/Bow/Curve_Aiming.Curve_Aiming'");
	Curve = CurveAsset.Object;

	ConstructorHelpers::FClassFinder<UCUserWidget_CrossHair> asset(L"/Script/UMGEditor.WidgetBlueprint'/Game/Widget/WB_CrossHair.WB_CrossHair_C'");
	CrossHairClass = asset.Class;
}

void UCSkills_Bow_Zomming::Skill_Pressed()
{
	if (SpringArm == nullptr)
		return;
	if (Camera == nullptr)
		return;

	Super::Skill_Pressed();

	CrossHair = Cast<UCUserWidget_CrossHair>(CreateWidget(Owner->GetWorld(), CrossHairClass));
	if (CrossHair != nullptr)
	{
		bAniming = true;
		CrossHair->AddToViewport();
		CrossHair->SetVisibility(ESlateVisibility::Visible);
	}

	State->OnSkillMode();

	OriginData.TargetArmLength = SpringArm->TargetArmLength;
	OriginData.SocketOffset = SpringArm->SocketOffset;
	OriginData.bEnableCameraLag = SpringArm->bEnableCameraLag;
	OriginData.CameraLocation = Camera->GetRelativeLocation();

	SpringArm->TargetArmLength = AimData.TargetArmLength;
	SpringArm->SocketOffset = AimData.SocketOffset;
	SpringArm->bEnableCameraLag = AimData.bEnableCameraLag;
	Camera->SetRelativeLocation(AimData.CameraLocation);

	Timeline.PlayFromStart();
}

void UCSkills_Bow_Zomming::Skill_Released()
{
	if (SpringArm == nullptr)
		return;
	if (Camera == nullptr)
		return;

	Super::Skill_Released();
	
	bAniming = false;
	CrossHair->SetVisibility(ESlateVisibility::Hidden);

	State->OffSkillMode();

	SpringArm->TargetArmLength = OriginData.TargetArmLength;
	SpringArm->SocketOffset = OriginData.SocketOffset;
	SpringArm->bEnableCameraLag = OriginData.bEnableCameraLag;
	Camera->SetRelativeLocation(OriginData.CameraLocation);

	Timeline.ReverseFromEnd();
}

void UCSkills_Bow_Zomming::BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment, UCDoAction* InDoAction)
{
	Super::BeginPlay(InOwner, InAttachment, InDoAction);

	SpringArm = Cast<USpringArmComponent>(InOwner->GetComponentByClass(USpringArmComponent::StaticClass()));
	Camera = Cast<UCameraComponent>(InOwner->GetComponentByClass(UCameraComponent::StaticClass()));

	FOnTimelineVector timeline;
	timeline.BindUFunction(this, "OnAiming");

	Timeline.AddInterpVector(Curve, timeline);
	Timeline.SetPlayRate(AimingSpeed);

	ACAttachment_Bow* bow = Cast<ACAttachment_Bow>(InAttachment);

	if (!!bow)
		Bend = bow->GetBend();
}

void UCSkills_Bow_Zomming::Tick_Implementation(float InDeltaTime)
{
	Super::Tick_Implementation(InDeltaTime);

	Timeline.TickTimeline(InDeltaTime);

	if (bAniming == false)
		return;

	TArray<AActor*> ignores;
	ignores.Add(Owner);

	FVector start = Owner->GetMesh()->GetSocketLocation("Hand_Bow_Right_Arrow");
	AActor* actor = Owner->GetController();
	FVector end = actor->GetActorForwardVector() * 3000.0f;
	end = start + end;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	FHitResult lineHitResult;
	UKismetSystemLibrary::LineTraceSingleForObjects(Owner->GetWorld(), start, end, ObjectTypes, false, ignores, DrawDebug, lineHitResult, true);
	if (lineHitResult.bBlockingHit == true)
	{
		ACharacter* hit = Cast<ACharacter>(lineHitResult.GetActor());
		if (hit != nullptr)
		{
			CrossHair->CrossHair_Red();
			return;
		}
	}

	CrossHair->CrossHair_White();
}

void UCSkills_Bow_Zomming::OnAiming(FVector Output)
{
	Camera->FieldOfView = Output.X;

	if (!!Bend)
		*Bend = Output.Y;
}
