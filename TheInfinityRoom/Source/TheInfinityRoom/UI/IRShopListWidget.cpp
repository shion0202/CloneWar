// Fill out your copyright notice in the Description page of Project Settings.

#include "IRShopListWidget.h"
#include "Components/Button.h"
#include "Components/ListView.h"
#include "Components/TextBlock.h"
#include "Engine/Font.h"
#include "GameData/IRGameSingleton.h"
#include "Game/IRGameModeBase.h"
#include "Player/IRSaveGame.h"
#include "Item/IRShopItemObject.h"

void UIRShopListWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (nullptr != BTN_SkinItem)
	{
		BTN_SkinItem->OnClicked.AddUniqueDynamic(this, &UIRShopListWidget::OnClickSkinItem);
	}

	if (nullptr != BTN_HeadItem)
	{
		BTN_HeadItem->OnClicked.AddUniqueDynamic(this, &UIRShopListWidget::OnClickHeadItem);
	}

	if (nullptr != BTN_BackItem)
	{
		BTN_BackItem->OnClicked.AddUniqueDynamic(this, &UIRShopListWidget::OnClickBackItem);
	}

	if (nullptr != BTN_EffectItem)
	{
		BTN_EffectItem->OnClicked.AddUniqueDynamic(this, &UIRShopListWidget::OnClickEffectItem);
	}

	SetDefaultText();
	FLinearColor SelectedColor = FLinearColor::Yellow;
	TXT_SkinItem->SetColorAndOpacity(SelectedColor);

	ItemTable = UIRGameSingleton::Get().GetSkinItems();
	SetShopItems();

	CurrentState = EShopState::Skin;
}

void UIRShopListWidget::OnClickSkinItem()
{
	if (CurrentState == EShopState::Skin)
	{
		return;
	}

	SetDefaultText();
	FLinearColor SelectedColor = FLinearColor::Yellow;
	TXT_SkinItem->SetColorAndOpacity(SelectedColor);

	ItemTable = UIRGameSingleton::Get().GetSkinItems();
	SetShopItems();

	CurrentState = EShopState::Skin;
}

void UIRShopListWidget::OnClickHeadItem()
{
	if (CurrentState == EShopState::Head)
	{
		return;
	}

	SetDefaultText();
	FLinearColor SelectedColor = FLinearColor::Yellow;
	TXT_HeadItem->SetColorAndOpacity(SelectedColor);

	ItemTable = UIRGameSingleton::Get().GetHeadItems();
	SetShopItems();

	CurrentState = EShopState::Head;
}

void UIRShopListWidget::OnClickBackItem()
{
	if (CurrentState == EShopState::Back)
	{
		return;
	}

	SetDefaultText();
	FLinearColor SelectedColor = FLinearColor::Yellow;
	TXT_BackItem->SetColorAndOpacity(SelectedColor);

	ItemTable = UIRGameSingleton::Get().GetBackItems();
	SetShopItems();

	CurrentState = EShopState::Back;
}

void UIRShopListWidget::OnClickEffectItem()
{
	if (CurrentState == EShopState::Effect)
	{
		return;
	}

	SetDefaultText();
	FLinearColor SelectedColor = FLinearColor::Yellow;
	TXT_EffectItem->SetColorAndOpacity(SelectedColor);

	ItemTable = UIRGameSingleton::Get().GetEffectItems();
	SetShopItems();

	CurrentState = EShopState::Effect;
}

void UIRShopListWidget::SetShopItems()
{
	AIRGameModeBase* GameModeBase = Cast<AIRGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameModeBase)
	{
		UIRSaveGame* SaveGameInstance = GameModeBase->GetSaveGameInstance();
		LV_ShopItemList->ClearListItems();

		for (FIRItem Item : ItemTable)
		{
			UIRShopItemObject* ItemObject = NewObject<UIRShopItemObject>();
			ItemObject->SetItemData(Item);

			if (SaveGameInstance->Inventory.Contains(Item.ItemName))
			{
				// bool* Value = SaveGameInstance->Inventory.Find(Item.ItemName);
				ItemObject->SetIsPurchased(true);
			}

			LV_ShopItemList->AddItem(ItemObject);
		}
	}
}

void UIRShopListWidget::SetDefaultText()
{
	FLinearColor DefaultColor = FLinearColor::White;
	TXT_SkinItem->SetColorAndOpacity(DefaultColor);
	TXT_HeadItem->SetColorAndOpacity(DefaultColor);
	TXT_BackItem->SetColorAndOpacity(DefaultColor);
	TXT_EffectItem->SetColorAndOpacity(DefaultColor);
}
