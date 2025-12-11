// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "CUserWidget_Title.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UCUserWidget_Title : public UUserWidget
{
	GENERATED_BODY()
private:
	UButton* btnNewGame;
	UButton* btnExit;

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnStartGameClicked();
	UFUNCTION()
	void OnExitClicked();
};
