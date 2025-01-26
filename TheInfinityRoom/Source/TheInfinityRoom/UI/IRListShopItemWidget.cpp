// Fill out your copyright notice in the Description page of Project Settings.

#include "IRListShopItemWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Player/IRUIPlayerController.h"
#include "Game/IRGameModeBase.h"
#include "Player/IRSaveGame.h"
#include "Item/IRShopItemObject.h"

void UIRListShopItemWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	Item = Cast<UIRShopItemObject>(ListItemObject);
	if (nullptr != Item)
	{
		TXT_ItemName->SetText(Item->GetItemData().DisplayName);
		IMG_ItemIcon->SetBrushFromTexture(Item->GetItemData().Icon);
		TXT_Apply->SetText(FText::FromString(FString::FromInt(Item->GetItemData().Cost)));
	}

	UpdateButton();
}

void UIRListShopItemWidget::OnEquipItem()
{
	Item->SetIsPurchased(true);
	EquipItemActions[(uint8)Item->GetItemData().ItemType].EquipItemDelegate.ExecuteIfBound();
}

void UIRListShopItemWidget::NativeConstruct()
{
	if (nullptr != BTN_Preview)
	{
		BTN_Preview->OnClicked.AddUniqueDynamic(this, &UIRListShopItemWidget::OnPreviewClick);
	}

	if (nullptr != BTN_Apply)
	{
		BTN_Apply->OnClicked.AddUniqueDynamic(this, &UIRListShopItemWidget::OnApplyClick);
	}

	EquipItemActions.Add(FFOnEquipItemDelegateWrapper(FOnEquipItemDelegate::CreateUObject(
		this, &UIRListShopItemWidget::EquipSkinItem)));
	EquipItemActions.Add(FFOnEquipItemDelegateWrapper(FOnEquipItemDelegate::CreateUObject(
		this, &UIRListShopItemWidget::EquipHeadItem)));
	EquipItemActions.Add(FFOnEquipItemDelegateWrapper(FOnEquipItemDelegate::CreateUObject(
		this, &UIRListShopItemWidget::EquipBackItem)));
	EquipItemActions.Add(FFOnEquipItemDelegateWrapper(FOnEquipItemDelegate::CreateUObject(
		this, &UIRListShopItemWidget::EquipEffectItem)));
}

void UIRListShopItemWidget::OnPreviewClick()
{
	PreviewItem();
}

void UIRListShopItemWidget::OnApplyClick()
{
	AIRGameModeBase* GameModeBase = Cast<AIRGameModeBase>(GetWorld()->GetAuthGameMode());
	UIRSaveGame* SaveGameInstance = GameModeBase->GetSaveGameInstance();

	if (Item->IsPurchased())
	{
		OnEquipItem();
		PreviewItem();
	}
	else
	{
		if (SaveGameInstance->MoneyAmount < Item->GetItemData().Cost)
		{
			return;
		}
		
		Item->SetIsPurchased(true);
		SaveGameInstance->Inventory.Add(Item->GetItemData().ItemName, true);
		SaveGameInstance->MoneyAmount -= Item->GetItemData().Cost;

		AIRUIPlayerController* PlayerController = Cast<AIRUIPlayerController>(GetWorld()->GetFirstPlayerController());
		if (PlayerController)
		{
			PlayerController->UpdateShopMoney(SaveGameInstance->MoneyAmount);
			PlayerController->UseMoney(Item->GetItemData().Cost);
			PlayerController->UseShop();
		}

		UpdateButton();
	}
	GameModeBase->SaveGameData();
}

void UIRListShopItemWidget::UpdateButton()
{
	AIRGameModeBase* GameModeBase = Cast<AIRGameModeBase>(GetWorld()->GetAuthGameMode());
	UIRSaveGame* SaveGameInstance = GameModeBase->GetSaveGameInstance();

	if (Item->IsPurchased())
	{	
		TXT_PurchaseFront->SetText(FText::FromString(TEXT("")));
		TXT_PurchaseBack->SetText(FText::FromString(TEXT("")));

		if (SaveGameInstance->CurrentLanguage.Equals(TEXT("ko")))
		{
			TXT_Apply->SetText(FText::FromString(TEXT("착용")));
		}
		else
		{
			TXT_Apply->SetText(FText::FromString(TEXT("Equip")));
		}
	}
	else
	{
		if (SaveGameInstance->CurrentLanguage.Equals(TEXT("ko")))
		{
			TXT_PurchaseFront->SetText(FText::FromString(TEXT("구매 (")));
			TXT_PurchaseBack->SetText(FText::FromString(TEXT("원)")));
		}
		else
		{
			TXT_PurchaseFront->SetText(FText::FromString(TEXT("Buy (")));
			TXT_PurchaseBack->SetText(FText::FromString(TEXT(" Dollars)")));
		}
	}
}

void UIRListShopItemWidget::PreviewItem()
{
	AIRUIPlayerController* PlayerController = Cast<AIRUIPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		PlayerController->PreviewItem(Item->GetItemData());
	}
}

void UIRListShopItemWidget::EquipSkinItem()
{
	AIRGameModeBase* GameModeBase = Cast<AIRGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameModeBase)
	{
		UIRSaveGame* SaveGameInstance = GameModeBase->GetSaveGameInstance();
		SaveGameInstance->EquipedSkin = Item->GetItemData().SkeletalMesh;
		SaveGameInstance->HeadSocketName = Item->GetItemData().HeadSocketName;
	}
}

void UIRListShopItemWidget::EquipHeadItem()
{
	AIRGameModeBase* GameModeBase = Cast<AIRGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameModeBase)
	{
		UIRSaveGame* SaveGameInstance = GameModeBase->GetSaveGameInstance();
		SaveGameInstance->EquipedItems.Remove(TEXT("Head"));
		SaveGameInstance->EquipedItems.Add(TEXT("Head"), Item->GetItemData().StaticMesh);
	}
}

void UIRListShopItemWidget::EquipBackItem()
{
	AIRGameModeBase* GameModeBase = Cast<AIRGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameModeBase)
	{
		UIRSaveGame* SaveGameInstance = GameModeBase->GetSaveGameInstance();
		SaveGameInstance->EquipedItems.Remove(TEXT("Back"));
		SaveGameInstance->EquipedItems.Add(TEXT("Back"), Item->GetItemData().StaticMesh);
	}
}

void UIRListShopItemWidget::EquipEffectItem()
{
	AIRGameModeBase* GameModeBase = Cast<AIRGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameModeBase)
	{
		UIRSaveGame* SaveGameInstance = GameModeBase->GetSaveGameInstance();
		SaveGameInstance->EquipedEffect = Item->GetItemData().Effect;
	}
}
