#include "CUserWidget_Result.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"


void UCUserWidget_Result::BiondGameState()
{

}

void UCUserWidget_Result::NativeConstruct()
{
	Super::NativeConstruct();

	Result = Cast<UTextBlock>(GetWidgetFromName(L"txtResult"));
	if(Result != nullptr)
		Result->SetText(FText::FromString(L""));
	TitleB = Cast<UButton>(GetWidgetFromName(L"btnRetryGame"));
	if (TitleB != nullptr)
		TitleB->OnClicked.AddDynamic(this, &UCUserWidget_Result::OnTitleClicked);
}

void UCUserWidget_Result::OnTitleClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), L"Title", true);
}

void UCUserWidget_Result::GameResult(bool InAmount)
{
	if (InAmount == true)
		Result->SetText(FText::FromString(L"Player Dead"));
	else
		Result->SetText(FText::FromString(L"Boss Dead"));
}
