// Fill out your copyright notice in the Description page of Project Settings.


#include "CUserWidget_Title.h"

#include "Engine/LevelStreamingDynamic.h"
#include "Kismet/GameplayStatics.h"

void UCUserWidget_Title::NativeConstruct()
{
	Super::NativeConstruct();
	//UGameplayStatics::LoadGameFromSlot(L"Player", 0);

	btnNewGame = Cast<UButton>(GetWidgetFromName(TEXT("btnNewGame")));
	if (btnNewGame != nullptr)
		btnNewGame->OnClicked.AddDynamic(this, &UCUserWidget_Title::OnStartGameClicked);

	btnExit = Cast<UButton>(GetWidgetFromName(TEXT("btnExit")));
	if (btnExit != nullptr)
		btnExit->OnClicked.AddDynamic(this, &UCUserWidget_Title::OnExitClicked);
}

void UCUserWidget_Title::OnStartGameClicked()
{
	//FLatentActionInfo LatentInfo;
	//LatentInfo.CallbackTarget = this;
	//LatentInfo.ExecutionFunction = FName("OnLevelLoaded");
	//LatentInfo.Linkage = 0;
	//LatentInfo.UUID = __LINE__;
	//// 비동기 레벨 로드
	//UGameplayStatics::LoadStreamLevel(GetWorld(), FName("NewMap"), true, false, FLatentActionInfo());

	UGameplayStatics::OpenLevel(GetWorld(), L"NewMap", true);
}

void UCUserWidget_Title::OnExitClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, false);
}
