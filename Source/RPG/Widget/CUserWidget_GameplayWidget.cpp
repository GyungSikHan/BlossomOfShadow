#include "CUserWidget_GameplayWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "RPG/Characters/CPlayer.h"

void UCUserWidget_GameplayWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ResumeButton = Cast<UButton>(GetWidgetFromName(TEXT("btnResume")));
	if (ResumeButton != nullptr)
		ResumeButton->OnClicked.AddDynamic(this, &UCUserWidget_GameplayWidget::OnResumeClicked);

	ReturnToTitleButton = Cast<UButton>(GetWidgetFromName(TEXT("btnReturnToTitle")));
	if (ReturnToTitleButton != nullptr)
		ReturnToTitleButton->OnClicked.AddDynamic(this, &UCUserWidget_GameplayWidget::OnReturnToTitleClicked);
}

void UCUserWidget_GameplayWidget::OnResumeClicked()
{
	APlayerController* controller = Cast<APlayerController>(GetOwningPlayer());
	if (controller == nullptr)
		return;
	ACPlayer* player = Cast<ACPlayer>(controller->GetPawn());

	RemoveFromParent();
	player->ChangeInputMode(true);
	controller->SetPause(false);
}

void UCUserWidget_GameplayWidget::OnReturnToTitleClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), L"Title");
}
