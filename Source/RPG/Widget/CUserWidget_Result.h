// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CUserWidget_GameplayWidget.h"
#include "Components/TextBlock.h"
#include "CUserWidget_Result.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UCUserWidget_Result : public UCUserWidget_GameplayWidget
{
	GENERATED_BODY()

public:
	void BiondGameState();

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnTitleClicked();

public:
	void GameResult(bool InAmount);

private:
	UTextBlock* Result;
	UButton* TitleB;
};

