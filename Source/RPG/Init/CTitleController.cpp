#include "CTitleController.h"

#include "Blueprint/UserWidget.h"

void ACTitleController::BeginPlay()
{
	Super::BeginPlay();

	if(UIWidgetClass == nullptr)
		return;

	UIWidget = CreateWidget<UUserWidget>(this, UIWidgetClass);
	if(UIWidget == nullptr)
		return;

	UIWidget->AddToViewport();

	FInputModeUIOnly mode;
	mode.SetWidgetToFocus(UIWidget->GetCachedWidget());
	SetInputMode(mode);
	bShowMouseCursor = true;
}
