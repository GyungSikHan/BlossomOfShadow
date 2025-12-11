#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_MenuButton.generated.h"

UCLASS()
class RPG_API UCUserWidget_MenuButton : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;

public:
	FORCEINLINE class UButton* GetButton() { return Image; }
	FORCEINLINE class UBorder* GetBorder() { return Select; }

private:
	UFUNCTION()
	void OnHovered();
	UFUNCTION()
	void OnUnhovered();
	UFUNCTION()
	void OnClicked();

private:
	UButton* Image;
	class UBorder* Select;
};
