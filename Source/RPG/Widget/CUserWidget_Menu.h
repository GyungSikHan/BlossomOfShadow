// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_Menu.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UCUserWidget_Menu : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;

public:
	void OnHovered(FString InName);
	void OnUnhovered(FString InName);
	void OnClicked(FString InName);

private:
	FORCEINLINE	void OnHovered_Break(){};
	FORCEINLINE void OnUnhovered_Break(){};
	FORCEINLINE void OnClicked(){};

private:
	class UGridPanel* Grid;
public:
	// 블루프린트에서 정의된 Textures 배열
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Textures")
	TArray<class UTexture2D*> Textures;

	TArray<UTexture2D*> GetTexturesFromBlueprint() const;
};




