#include "CGameInstance.h"

#include "Characters/CPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Widget/CUserWidget_PlayerInterface.h"
#include "Widget/CUserWidget_Result.h"

void UCGameInstance::Init()
{
	Super::Init();
	IsReset = false;
}

void UCGameInstance::SetReset(bool InAmount)
{
	IsReset = InAmount;
}

void UCGameInstance::UpdatePlayerHP(UWorld* InWorld, float InHealth, float InMaxHealth)
{
	ACPlayer* player = GetPlayer(InWorld);
	if (player == nullptr)
		return;

	player->GetUI()->UpdateHealth(InHealth, InMaxHealth);
}

void UCGameInstance::UpdatePlayerMP(UWorld* InWorld, float InMP, float InMaxMP)
{
	ACPlayer* player = GetPlayer(InWorld);
	if (player == nullptr)
		return;

	player->GetUI()->UpdateMP(InMP, InMaxMP);
}

void UCGameInstance::UpdatePlayerStamina(UWorld* InWorld, float InStamina, float InMaxStamina)
{
	ACPlayer* player = GetPlayer(InWorld);
	if (player == nullptr)
		return;

	player->GetUI()->UpdateStamina(InStamina, InMaxStamina);
}

void UCGameInstance::ShowBossUI(UWorld* InWorld, bool IsTrue)
{
	ACPlayer* player = GetPlayer(InWorld);
	if (player == nullptr)
		return;
	
	player->GetUI()->UpdateSeeBoss(IsTrue, "Boss");
}

void UCGameInstance::UpdateBossUI(UWorld* InWorld, float InHealth, float InMaxHealth)
{
	ACPlayer* player = GetPlayer(InWorld);
	if(player == nullptr)
		return;
	player->GetUI()->UpdateBossHealth(InHealth, InMaxHealth);
}

ACPlayer* UCGameInstance::GetPlayer(UWorld* InWorld)
{
	APlayerController* controller = UGameplayStatics::GetPlayerController(InWorld, 0);
	if (controller == nullptr)
		return nullptr;
	ACPlayer* player = Cast<ACPlayer>(controller->GetPawn());
	if (player == nullptr)
		return nullptr;

	return player;
}

void UCGameInstance::ShowResultUI(UWorld* InWorld, bool bPlayer)
{
	APlayerController* controller = UGameplayStatics::GetPlayerController(InWorld, 0);
	if (controller == nullptr)
		return;

	ResultWidget = Cast<UCUserWidget_Result>(CreateWidget(InWorld, ResultWidgetClass));
	ResultWidget->AddToViewport();
	ResultWidget->GameResult(bPlayer);
	FInputModeUIOnly UIInputMode;
	controller->SetPause(true);
	controller->SetInputMode(UIInputMode);
	controller->bShowMouseCursor = true;
}
