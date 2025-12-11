#include "CUserWidget_MenuButton.h"

#include "CUserWidget_Menu.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "RPG/Characters/CPlayer.h"

void UCUserWidget_MenuButton::NativeConstruct()
{
	Super::NativeConstruct();
	Image = Cast<UButton>(GetWidgetFromName(L"ImageButton"));
	if (Image == nullptr)
		return;
	Select = Cast<UBorder>(GetWidgetFromName(L"Select"));
	if(Select == nullptr)
		return;

	Image->OnHovered.AddDynamic(this, &UCUserWidget_MenuButton::OnHovered);
	Image->OnUnhovered.AddDynamic(this, &UCUserWidget_MenuButton::OnUnhovered);
	Image->OnClicked.AddDynamic(this, &UCUserWidget_MenuButton::OnClicked);
}

void UCUserWidget_MenuButton::OnHovered()
{
	ACPlayer* player = Cast<ACPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	player->GetMeun()->OnHovered(this->GetName());
}

void UCUserWidget_MenuButton::OnUnhovered()
{
	ACPlayer* player = Cast<ACPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	player->GetMeun()->OnUnhovered(this->GetName());
}

void UCUserWidget_MenuButton::OnClicked()
{

	ACPlayer* player = Cast<ACPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	player->GetMeun()->OnClicked(this->GetName());
}
