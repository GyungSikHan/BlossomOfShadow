#include "CEnemy_AI.h"

#include "CAIController.h"
#include "RPG/Utilities/CLog.h"
#include "Characters/CPlayer.h"
#include "Components/CAIBehaviorComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Init/CGameInstance.h"
#include "Item/CDrop_Item.h"
#include "Kismet/GameplayStatics.h"
#include "RPG/Components/CMovementComponent.h"
#include "RPG/Components/CStateComponent.h"
#include "RPG/Components/CMontagesComponent.h"
#include "RPG/Components/CStatusComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "RPG/Weapon/CWeaponStructures.h"

#include "Perception/AISense_Damage.h"
#include "RPG/Widget/CUserWidget_PlayerInterface.h"
#include "Weapon/CAttachment.h"
#include "Weapon/CDoAction.h"

ACEnemy_AI::ACEnemy_AI()
{
	PrimaryActorTick.bCanEverTick = true;

	TeamID = 2;
	Team = TeamID;
	Behavior = this->CreateDefaultSubobject<UCAIBehaviorComponent>("Behavior");

	Widget = this->CreateDefaultSubobject<UWidgetComponent>("Label");
	Widget->SetupAttachment(GetMesh());
	Widget->SetRelativeLocation(FVector(0, 0, 220));
	Widget->SetDrawSize(FVector2D(200, 0));
	Widget->SetWidgetSpace(EWidgetSpace::Screen);
}

void ACEnemy_AI::BeginPlay()
{
	Super::BeginPlay();

	if (LabelWidgetClass != nullptr)
		Widget->SetWidgetClass(LabelWidgetClass);
	
	AIController = Cast<ACAIController>(GetController());
	UCUserWidget_PlayerInterface* label = Cast<UCUserWidget_PlayerInterface>(Widget->GetUserWidgetObject());
	if(label == nullptr)
		return;
	label->UpdateHealth(Status->GetHealth(), Status->GetMaxHealth());
}

void ACEnemy_AI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UCUserWidget_PlayerInterface* label = Cast<UCUserWidget_PlayerInterface>(Widget->GetUserWidgetObject());
	if(label != nullptr)
	{
		label->UpdateHealth(Status->GetHealth(), Status->GetMaxHealth());
		UpdateLabelRenderScale();
	}
	if(bBoss == true)
	{
		ACPlayer* player = Cast<ACPlayer>(Behavior->GetTarget());
		if(player != nullptr)
		{
			if(bTest == false)
			{
				bTest = true;
				GameInstance->ShowBossUI(GetWorld(),true);
			}
			GameInstance->UpdateBossUI(GetWorld(), Status->GetHealth(), Status->GetMaxHealth());
		}
	}
}

void ACEnemy_AI::End_Dead()
{
	if(bBoss == true)
		GameInstance->ShowBossUI(GetWorld(), false);
	
	Super::End_Dead();
	if (DropItem != nullptr)
		Drop_Item();
}

void ACEnemy_AI::UpdateLabelRenderScale()
{
	UCUserWidget_PlayerInterface* label = Cast<UCUserWidget_PlayerInterface>(Widget->GetUserWidgetObject());
	if (label == nullptr)
	{
		CLog::Print("Test");
		return;
	}


	APlayerCameraManager* cameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);

	FVector cameraLocation = cameraManager->GetCameraLocation();
	FVector targetLocation = GetController()->GetTargetLocation();

	float distance = FVector::Distance(cameraLocation, targetLocation);
	float sizeRate = 1.0f - (distance / LabelViewAmount);

	if (distance > LabelViewAmount)
	{
		label->SetVisibility(ESlateVisibility::Collapsed);

		return;
	}

	label->SetVisibility(ESlateVisibility::Visible);
	label->SetRenderScale(FVector2D(sizeRate, sizeRate));
}

void ACEnemy_AI::Drop_Item()
{
	FVector start = GetActorLocation();
	FVector end = start - FVector(0,0,200);
	FHitResult result;
	TArray<AActor*> ignore;
	ignore.AddUnique(this);

	bool bHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), start, end, ETraceTypeQuery::TraceTypeQuery1, false, ignore, DropItemDebug, result, true);

	FActorSpawnParameters param{};
	param.Owner = this;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	FTransform transform = UKismetMathLibrary::MakeTransform(result.Location, UKismetMathLibrary::MakeRotator(0.0f, 0.0f, UKismetMathLibrary::RandomFloatInRange(0.0f, 360.0f)));
	AActor* ac = GetWorld()->SpawnActor<ACDrop_Item>(DropItem, transform,param);
}

void ACEnemy_AI::Dead()
{
	Widget->SetVisibility(false);
	Super::Dead();
}

//void ACEnemy_AI::SetHammerMode()
//{
//	Weapon->SetHammerMode();
//}
//
//void ACEnemy_AI::SetRodMode()
//{
//	Weapon->SetMagicRodMode();
//}