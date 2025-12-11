// Fill out your copyright notice in the Description page of Project Settings.


#include "CUserWidget_Menu.h"
#include "RPG/Utilities/CLog.h"
#include "CUserWidget_MenuButton.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/GridPanel.h"
#include "Brushes/SlateImageBrush.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/DefaultValueHelper.h"
#include "RPG/Characters/CPlayer.h"
#include "RPG/Components/CWeaponComponent.h"
#include "RPG/Weapon/CWeaponAsset.h"

void UCUserWidget_Menu::NativeConstruct()
{
	Super::NativeConstruct();
	Grid = Cast<UGridPanel>(GetWidgetFromName(L"Grid"));
	if (Grid == nullptr)
		return;
	TArray<UTexture2D*> textures = GetTexturesFromBlueprint();
	

	int32 index{};
	for (UWidget* Widget : Grid->GetAllChildren())
	{
		// Cast로 UCUserWidget_MenuButton 타입으로 변환
		UCUserWidget_MenuButton* Button = Cast<UCUserWidget_MenuButton>(Widget);
		if (Button && Button->GetButton())  // Button과 GetButton()이 유효한지 확인
		{
			// 현재 버튼의 스타일을 가져옴
			FButtonStyle Style = Button->GetButton()->GetStyle();

			Style.Normal.SetResourceObject(textures[index]);
			Style.Hovered.SetResourceObject(textures[index]);
			Style.Pressed.SetResourceObject(textures[index]);

			// 변경된 스타일을 버튼에 다시 설정
			Button->GetButton()->SetStyle(Style);

			index++;
		}
	}
}

void UCUserWidget_Menu::OnHovered(FString InName)
{
	for (UWidget* widget : Grid->GetAllChildren())
	{
		if(widget->GetName() == InName)
		{
			Cast<UCUserWidget_MenuButton>(widget)->GetBorder()->SetBrushColor(FLinearColor(1, 0, 0, 1));

			OnHovered_Break();
			break;
		}
	}
}

void UCUserWidget_Menu::OnUnhovered(FString InName)
{
	for (UWidget* widget : Grid->GetAllChildren())
	{
		if (widget->GetName() == InName)
		{
			Cast<UCUserWidget_MenuButton>(widget)->GetBorder()->SetBrushColor(FLinearColor(1, 1, 1, 1));

			OnUnhovered_Break();
			break;
		}
	}
}

void UCUserWidget_Menu::OnClicked(FString InName)
{
	for (UWidget* widget : Grid->GetAllChildren())
	{
		if (widget->GetName() == InName)
		{
			ACPlayer* player = Cast<ACPlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());
			player->MenuHide();
			UCWeaponComponent* weapon = Cast<UCWeaponComponent>(player->GetComponentByClass(UCWeaponComponent::StaticClass()));
			int32 result{};
			FDefaultValueHelper::ParseInt(InName.Replace(L"Action_", L"", ESearchCase::IgnoreCase), result);

			switch (result)
			{
			case 0:
				weapon->SetSwordMode();
				break;
			case 1:
				weapon->SetHammerMode();
				break;
			case 2:
				weapon->SetMagicRodMode();
				break;
			case 3:
				weapon->SetBowMode();
				break;
			case 4:
				weapon->SetWarpMode();
			default:
				break;
			}

		}
	}
}

TArray<UTexture2D*> UCUserWidget_Menu::GetTexturesFromBlueprint() const
{
	return Textures;
}