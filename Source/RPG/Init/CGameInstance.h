// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UCGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
    // 플레이어 체력을 저장하는 변수
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player Stats")
    float PlayerHealth;
public:
    // 초기화 함수
    virtual void Init() override;
    void SetReset(bool InAmount);
public:
    FORCEINLINE bool GetReset() { return IsReset; }

    void UpdatePlayerHP(UWorld* InWorld, float InHealth, float InMaxHealth);
    void UpdatePlayerMP(UWorld* InWorld, float InMP, float InMaxMP);
    void UpdatePlayerStamina(UWorld* InWorld, float InStamina, float InMaxStamina);

    void ShowBossUI(UWorld* InWorld, bool IsTrue);
    void UpdateBossUI(UWorld* InWorld, float InHealth, float InMaxHealth);

private:
    class ACPlayer* GetPlayer(UWorld* InWorld);
private:
    bool IsReset;

public:
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "UI")
    TSubclassOf<class UCUserWidget_Result> ResultWidgetClass;
private:
	UPROPERTY()
    class UCUserWidget_Result* ResultWidget;

public:
    void ShowResultUI(UWorld* InWorld, bool bPlayer);
};
