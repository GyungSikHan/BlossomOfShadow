#include "CPlayer.h"
#include "RPG/Utilities/CLog.h"
#include "Init/CGameInstance.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "RPG/Components/CMovementComponent.h"
#include "RPG/Components/CMontagesComponent.h"
#include "RPG/Components/CWeaponComponent.h"
#include "RPG/Components/CStatusComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "RPG/Widget/CUserWidget_PlayerInterface.h"
#include "EnhancedInputComponent.h"
#include "RPG/Components/CTargetComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AI/CEnemy_AI.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/ArrowComponent.h"
#include "Components/CFeetComponent.h"
#include "RPG/Components/CZoomComponent.h"
#include "RPG/Weapon/CDoAction.h"
#include "RPG/Widget/CUserWidget_GameplayWidget.h"
#include "RPG/Widget/CUserWidget_Menu.h"
#include "RPG/Widget/CUserWidget_Result.h"
#include "UObject/ConstructorHelpers.h"
#include "Weapon/Skills/CSkills.h"

ACPlayer::ACPlayer()
{
	PrimaryActorTick.bCanEverTick = true;
	
	TeamID = 1;

	//springarm
	{
		SpringArm = this->CreateDefaultSubobject<USpringArmComponent>("SpringArm");
		SpringArm->SetupAttachment(GetCapsuleComponent());
	}
	//camera
	{
		Camera = this->CreateDefaultSubobject<UCameraComponent>("Camera");
		Camera->SetupAttachment(SpringArm);

		Camera->SetActive(true);
		
	}

	//Component setting
	{
		Feet = CreateDefaultSubobject<UCFeetComponent>("Feet");
		Zoom = CreateDefaultSubobject<UCZoomComponent>("Zoom");
		//
		Parkour = CreateDefaultSubobject<UCParkourComponent>("Parkour");
		ArrowGroup = CreateDefaultSubobject<USceneComponent>("ParkourArrowGroup");
		ArrowGroup->SetupAttachment(GetCapsuleComponent());
		for (int32 i = 0; i < (int32)EParkourType::Max; i++)
		{
			FString name = StaticEnum<EParkourArrowType>()->GetNameStringByIndex(i);
			ParkourArrows[i] = CreateDefaultSubobject<UArrowComponent>(FName(name));
			ParkourArrows[i]->SetupAttachment(ArrowGroup);
			switch ((EParkourArrowType)i)
			{
			case EParkourArrowType::Center:
				ParkourArrows[i]->ArrowColor = FColor::Red;
				break;

			case EParkourArrowType::Ceil:
				ParkourArrows[i]->ArrowColor = FColor::Green;
				ParkourArrows[i]->SetRelativeLocation(FVector(0, 0, 100));
				break;

			case EParkourArrowType::Floor:
				ParkourArrows[i]->ArrowColor = FColor::Blue;
				ParkourArrows[i]->SetRelativeLocation(FVector(0, 0, -80));
				break;

			case EParkourArrowType::Left:
				ParkourArrows[i]->ArrowColor = FColor::Magenta;
				ParkourArrows[i]->SetRelativeLocation(FVector(0, -30, 0));
				break;

			case EParkourArrowType::Right:
				ParkourArrows[i]->ArrowColor = FColor::Magenta;
				ParkourArrows[i]->SetRelativeLocation(FVector(0, 30, 0));
				break;

			case EParkourArrowType::Land:
				ParkourArrows[i]->ArrowColor = FColor::Yellow;
				ParkourArrows[i]->SetRelativeLocation(FVector(200, 0, 100));
				ParkourArrows[i]->SetRelativeRotation(FRotator(-90, 0, 0));
				break;
			}
		}

	}

	////Arrow
	//{
	//	Arrow->ArrowColor = FColor::Blue;
	//	Arrow->SetRelativeLocation(FVector(200, 0, 0));
	//	Arrow->SetRelativeRotation(FRotator(-90, 0, 0));
	//}

	


	SpringArm->SetRelativeLocation(FVector(0, 40, 60));
	SpringArm->SetRelativeRotation(FRotator(0, 90, 0));
	SpringArm->TargetArmLength = 200;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bEnableCameraLag = true;

	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);

	//Pause Widget
	{
		ConstructorHelpers::FClassFinder<UCUserWidget_GameplayWidget> UI_Menu(L"/Script/UMGEditor.WidgetBlueprint'/Game/Widget/UI_Menu.UI_Menu_C'");
		if (UI_Menu.Succeeded() == true)
			MenuWidgetClass = UI_Menu.Class;
	}
	//Result Widget
	{
		ConstructorHelpers::FClassFinder<UCUserWidget_Result> UI(L"/Script/UMGEditor.WidgetBlueprint'/Game/Widget/UI_Result.UI_Result_C'");
		if (UI.Succeeded() == true)
			ResultWidgetClass = UI.Class;
	}
	//WBMenu
	{
		ConstructorHelpers::FClassFinder<UCUserWidget_Menu> UI(L"/Script/UMGEditor.WidgetBlueprint'/Game/Widget/WB_Menu.WB_Menu_C'");
		if (UI.Succeeded() == true)
			WBMeunClass = UI.Class;
	}
}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();

	if (GameInstance->GetReset() == false)
	{
		SaveHP();
		GameInstance->SetReset(true);
	}
	else
		Restore();

	Move->OnRun();
	Move->DisableControlRotation();

	//Widget
	{
		LabelWidget = Cast<UCUserWidget_PlayerInterface>(CreateWidget(Owner->GetWorld(), LabelWidgetClass));
		LabelWidget->UpdateHealth(Status->GetHealth(), Status->GetMaxHealth());
		LabelWidget->UpdateMP(Status->GetMP(), Status->GetMaxMP());
		LabelWidget->UpdateStamina(Status->GetStamina(), Status->GetMaxStamina());
		LabelWidget->AddToViewport();
		LabelWidget->SetVisibility(ESlateVisibility::Visible);
	}

	//Result Widget
	{
		ResultWidget = Cast<UCUserWidget_Result>(CreateWidget(Owner->GetWorld(), ResultWidgetClass));
		ResultWidget->AddToViewport();
		ResultWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	//WBMenu
	{
		WBMeun = Cast<UCUserWidget_Menu>(CreateWidget(Owner->GetWorld(), WBMeunClass));
		WBMeun->AddToViewport();
		WBMeun->SetVisibility(ESlateVisibility::Hidden);
	}
	//ControllUI
	{
		if(ControllUIClass != nullptr)
		{
			UIControll = Cast<UUserWidget>(CreateWidget(Owner->GetWorld(), ControllUIClass));
			UIControll->AddToViewport();
			UIControll->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	Controller = Cast<APlayerController>(GetController());
	if (Controller == nullptr)
		return;
	
	UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(Controller->GetLocalPlayer());
	if (subsystem == nullptr)
		return;
	
	subsystem->AddMappingContext(DefaultMappingContext, 0);
	
	GetWorldTimerManager().SetTimer(Timer, this, &ACPlayer::RecoveryMPStamina, 1, true);

	LabelWidget->UpdateSeeBoss(false);
}

void ACPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Weapon->IsWarpMode()==true)
	{
		Move->DisableControlRotation();
	}

	//Widget
	GameInstance->UpdatePlayerHP(GetWorld(), Status->GetHealth(), Status->GetMaxHealth());
	GameInstance->UpdatePlayerMP(GetWorld(), Status->GetMP(), Status->GetMaxMP());
	GameInstance->UpdatePlayerStamina(GetWorld(), Status->GetStamina(), Status->GetMaxStamina());
	/*LabelWidget->UpdateHealth(Status->GetHealth(), Status->GetMaxHealth());
	LabelWidget->UpdateMP(Status->GetMP(), Status->GetMaxMP());
	LabelWidget->UpdateStamina(Status->GetStamina(), Status->GetMaxStamina());*/

	//MakeNoise
	if (bNoise == false)
		return;

	MakeNoise(1, nullptr, GetActorLocation());
	//UGameplayStatics::PlaySoundAtLocation(GetWorld(), , GetActorLocation(), GetActorRotation());

}

void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if(EnhancedInputComponent == nullptr)
		return;

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, Move, &UCMovementComponent::OnMove);
	EnhancedInputComponent->BindAction(Look, ETriggerEvent::Triggered, Move, &UCMovementComponent::OnLook);
	EnhancedInputComponent->BindAction(Zooming, ETriggerEvent::Triggered, this, &ACPlayer::SetZooming);

	EnhancedInputComponent->BindAction(Sword, ETriggerEvent::Triggered, Weapon, &UCWeaponComponent::SetSwordMode);
	EnhancedInputComponent->BindAction(Hammer, ETriggerEvent::Triggered, Weapon, &UCWeaponComponent::SetHammerMode);
	EnhancedInputComponent->BindAction(Rod, ETriggerEvent::Triggered, Weapon, &UCWeaponComponent::SetMagicRodMode);
	EnhancedInputComponent->BindAction(Bow, ETriggerEvent::Triggered, Weapon, &UCWeaponComponent::SetBowMode);
	EnhancedInputComponent->BindAction(Warp, ETriggerEvent::Triggered, Weapon, &UCWeaponComponent::SetWarpMode);

	EnhancedInputComponent->BindAction(Targetting, ETriggerEvent::Triggered, Target, &UCTargetComponent::Toggle_Target);
	EnhancedInputComponent->BindAction(ChangeTargetting, ETriggerEvent::Triggered, Target, &UCTargetComponent::ChangeFocus);

	EnhancedInputComponent->BindAction(Pause, ETriggerEvent::Triggered, this, &ACPlayer::OnGamePause);

	EnhancedInputComponent->BindAction(Menu, ETriggerEvent::Started, this, &ACPlayer::VisibleMenu);
	EnhancedInputComponent->BindAction(Menu, ETriggerEvent::Completed, this, &ACPlayer::MenuHide);
	EnhancedInputComponent->BindAction(ControllUI, ETriggerEvent::Started, this, &ACPlayer::VisibleControllMenu);
	EnhancedInputComponent->BindAction(ControllUI, ETriggerEvent::Completed, this, &ACPlayer::HidenUI);


	EnhancedInputComponent->BindAction(Sprint, ETriggerEvent::Started, this, &ACPlayer::MakeSound);
	EnhancedInputComponent->BindAction(Sprint, ETriggerEvent::Completed, this, &ACPlayer::MakeSound);

	EnhancedInputComponent->BindAction(Jumping, ETriggerEvent::Started, Move, &UCMovementComponent::OnJump);
	EnhancedInputComponent->BindAction(Jumping, ETriggerEvent::Completed, Move, &UCMovementComponent::OnEndJump);


	EnhancedInputComponent->BindAction(Avoid, ETriggerEvent::Triggered, this, &ACPlayer::OnAvoid);

	EnhancedInputComponent->BindAction(Action, ETriggerEvent::Triggered, Weapon, &UCWeaponComponent::DoAction);

	EnhancedInputComponent->BindAction(Skill1, ETriggerEvent::Started, this, &ACPlayer::Combo, ESkillsType::Skill1);
	EnhancedInputComponent->BindAction(Skill1, ETriggerEvent::Completed, Weapon, &UCWeaponComponent::Skills_Released);
	EnhancedInputComponent->BindAction(Skill2, ETriggerEvent::Started, this, &ACPlayer::Combo, ESkillsType::Skill2);
	EnhancedInputComponent->BindAction(Skill2, ETriggerEvent::Completed, Weapon, &UCWeaponComponent::Skills_Released);
	

}

void ACPlayer::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	Super::OnStateTypeChanged(InPrevType, InNewType);
	switch (InNewType)
	{
	case EStateType::BackStep:
		BackStep();
		break;
	case EStateType::Roll_F:
	case EStateType::Roll_B:
	case EStateType::Roll_L:
	case EStateType::Roll_R:
	case EStateType::Roll_FR:
	case EStateType::Roll_FL:
	case EStateType::Roll_BR:
	case EStateType::Roll_BL:
		Roll();
		break;
	default:
		break;
	}
}

void ACPlayer::OnAvoid()
{
	if (State->IsIdleMode() == false || Status->GetMP() <= 0 || Status->GetStamina() <= 0)
		return;

	bRoll = true;
	FVector2D MovementVector = Move->Values.Get<FVector2D>();

	if (MovementVector.Y > 0 && MovementVector.X != 0)
	{
		Axis = MovementVector.Y + MovementVector.X;
		if (Axis == 0)
			Axis += 4;
		else
			Axis += 3;
		State->SetRoll_FRL_Mode(Axis);
	}
	else if (MovementVector.Y < 0 && MovementVector.X != 0)
	{
		Axis = MovementVector.Y + MovementVector.X;
		if (Axis < 0)
			Axis += 8;
		else
			Axis += 7;
		State->SetRoll_BRL_Mode(Axis);
	}
	else if (MovementVector.Y != 0 && MovementVector.X == 0)
	{
		Axis = MovementVector.Y;
		if (Axis < 0)
			Axis += 1;
		State->SetRoll_FB_Mode(Axis);
	}
	else if (MovementVector.Y == 0 && MovementVector.X != 0)
	{
		Axis = MovementVector.X;
		if (Axis < 0)
			Axis += 3;
		else
			Axis += 2;
		State->SetRoll_RL_Mode(Axis);
	}
	else
		State->SetBackStepMode();

	MovementVector = FVector2D::ZeroVector;
}

void ACPlayer::BackStep()
{
	bRotation = Move->GetControlRotation();

	Move->EnableControlRotation();
	Montage->PlayBackStepMode();
}

void ACPlayer::Roll()
{
	bRotation = Move->GetControlRotation();

	Move->EnableControlRotation();
	Montage->PlayRollMode(Axis);
}

void ACPlayer::SetZooming(const FInputActionValue& Value)
{
	if (Weapon->IsBowMode() == true)
		return;
	float value = Value.Get<float>();
	Zoom->SetZoomValue(value);
}

void ACPlayer::End_BackStep()
{
	if (!!bRotation)
		Move->EnableControlRotation();
	else
		Move->DisableControlRotation();

	bRoll = false;
	State->SetIdleMode();
}

void ACPlayer::End_Roll()
{
	if (!!bRotation)
		Move->EnableControlRotation();
	else
		Move->DisableControlRotation();

	State->SetIdleMode();

	bRoll = false;
	Move->Values = FVector2D::ZeroVector;
}

void ACPlayer::UseMp(float InMP)
{
	IIMPStamina::UseMp(InMP);
	Status->UseMp(InMP);
}

void ACPlayer::UseStamina(float INStamina)
{
	IIMPStamina::UseStamina(INStamina);
	Status->UseStamina(INStamina);
}

void ACPlayer::Hitted()
{
	Super::Hitted();
	Move->DisableControlRotation();
}

void ACPlayer::Dead()
{
	Super::Dead();
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.2f);
}

void ACPlayer::End_Hitted()
{
	Super::End_Hitted();

	if (Weapon->GetSkills() != nullptr && Weapon->GetSkills()->GetInAction() == true)
		Weapon->GetSkills()->End_Skill_Implementation();
	if(bRoll == true)
		End_Roll();
	Move->Move();
	Move->EnableControlRotation();
}


void ACPlayer::End_Dead()
{
	GetWorldTimerManager().ClearTimer(Timer);
	//ShowResultUI(true);
	GameInstance->ShowResultUI(GetWorld(), true);
	Super::End_Dead();
}

void ACPlayer::ChangeTarget()
{
	Target->ChangeFocus(true);
}

void ACPlayer::RecoveryMPStamina()
{
	if (Status->GetMP() != Status->GetMaxMP())
		Status->RecorveryMp(0.5);
	if (Status->GetStamina() != Status->GetMaxStamina())
		Status->RecorveryStamina(0.7);
}

void ACPlayer::MakeSound()
{
	if (Status->GetMP() <= 0 || Status->GetStamina() <= 0)
		return;

	bNoise = !bNoise;
	if (bNoise == true)
		Move->OnSprint();
	else
		Move->OnRun();
}

void ACPlayer::TestDead()
{
	Status->Damage(1000);
	State->IsDeadMode();
	Dead();
}

void ACPlayer::OnGamePause()
{
	MenuWidget = Cast<UCUserWidget_GameplayWidget>(CreateWidget<UCUserWidget_GameplayWidget>(Owner->GetWorld(), MenuWidgetClass));
	if(MenuWidget == nullptr)
		 return;
	MenuWidget->AddToViewport(3);

	Controller->SetPause(true);
	ChangeInputMode(false);
}

void ACPlayer::ChangeInputMode(bool bGameMode)
{
	if (bGameMode == true)
	{
		Controller->SetInputMode(GameInputMode);
		Controller->bShowMouseCursor = false;
	}
	else
	{
		Controller->SetInputMode(UIInputMode);
		Controller->bShowMouseCursor = true;
	}
}

void ACPlayer::ShowResultUI(bool Result)
{
	ResultWidget->GameResult(Result);
	ResultWidget->SetVisibility(ESlateVisibility::Visible);
	Controller->SetPause(true);
	ChangeInputMode(false);
}

void ACPlayer::VisibleMenu()
{
	if(State->IsIdleMode() == false)
		return;

	WBMeun->SetVisibility(ESlateVisibility::Visible);
	FInputModeGameAndUI InputMode;
	InputMode.SetWidgetToFocus(WBMeun->TakeWidget());
	InputMode.SetHideCursorDuringCapture(false);
	Controller->SetInputMode(InputMode);
	Controller->bShowMouseCursor = true;
}

void ACPlayer::VisibleControllMenu()
{
	if(UIControll == nullptr)
		return;
	UIControll->SetVisibility(ESlateVisibility::Visible);
}

void ACPlayer::HidenUI()
{
	if (UIControll == nullptr)
		return;
	UIControll->SetVisibility(ESlateVisibility::Hidden);
}

void ACPlayer::MenuHide()
{
	WBMeun->SetVisibility(ESlateVisibility::Hidden);
	FInputModeGameOnly input;
	Controller->SetInputMode(input);
	Controller->bShowMouseCursor = false;
}

void ACPlayer::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	
	Parkour->DoParkour(true);
}

void ACPlayer::OnSkillButton(ESkillsType InSkillType)
{
}


void ACPlayer::Combo(ESkillsType InSkills)
{
	if(Weapon->IsUnarmedMode()  == true)
	{
		Parkour->DoParkour();
		return;
	}

	if(Status->GetMP() <= 0 || Status->GetStamina() <= 0)
		return;

	//bInterruptible = false;

	Weapon->Skills_Pressed(InSkills);
	//AirCollision->SetRelativeLocation(FVector(300, 0, 0));
}

void ACPlayer::SaveHP()
{
	GameInstance->PlayerHealth = Status->GetHealth();
}

void ACPlayer::Restore()
{
	Status->SetHP(GameInstance->PlayerHealth);
}
