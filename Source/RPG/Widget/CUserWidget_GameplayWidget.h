#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_GameplayWidget.generated.h"

UCLASS()
class RPG_API UCUserWidget_GameplayWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnResumeClicked();
	UFUNCTION()
	void OnReturnToTitleClicked();

protected:
	UPROPERTY()
	class UButton* ResumeButton;
	UPROPERTY()
	class UButton* ReturnToTitleButton;
};
