#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPG/Components/CStateComponent.h"
#include "CUserWidget_PlayerInterface.generated.h"

UCLASS()
class RPG_API UCUserWidget_PlayerInterface : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Label")
	void UpdateName(const FString& InName);
	UFUNCTION(BlueprintImplementableEvent, Category = "Label")
	void UpdateControllerName(const FString& InName);
	UFUNCTION(BlueprintImplementableEvent, Category = "Label")
	void UpdateHealth(float InHealth, float InMaxHealth);
	UFUNCTION(BlueprintImplementableEvent, Category = "Label")
	void UpdateMP(float InMp, float InMaxMp);
	UFUNCTION(BlueprintImplementableEvent, Category = "Label")
	void UpdateStamina(float InStamina, float InMaxStamina);
	UFUNCTION(BlueprintImplementableEvent, Category = "Label")
	void UpdateSeeBoss(bool IsBoss, const FString& InName = "");
	UFUNCTION(BlueprintImplementableEvent, Category = "Label")
	void UpdateBossHealth(float InHealth, float InMaxHealth);
};
