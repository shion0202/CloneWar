// Fill out your copyright notice in the Description page of Project Settings.

#include "IRListShopItemWidget.h"
#include "IRSkinItemObject.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "IRUIPlayerController.h"
#include "IRGameModeBase.h"
#include "IRSaveGame.h"

void UIRListShopItemWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	Item = Cast<UIRSkinItemObject>(ListItemObject);
	if (nullptr != Item)
	{
		TXT_ItemName->SetText(Item->GetSkinItemData().DisplayName);
		IMG_ItemIcon->SetBrushFromTexture(Item->GetSkinItemData().Icon);
		TXT_Apply->SetText(FText::FromString(FString::FromInt(Item->GetSkinItemData().Cost)));
	}

	UpdateButton();
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
}

void UIRListShopItemWidget::OnPreviewClick()
{
	PreviewMesh();
}

void UIRListShopItemWidget::OnApplyClick()
{
	AIRGameModeBase* GameModeBase = Cast<AIRGameModeBase>(GetWorld()->GetAuthGameMode());
	UIRSaveGame* SaveGameInstance = GameModeBase->GetSaveGameInstance();

	if (Item->IsPurchased())
	{
		SaveGameInstance->EquipedMesh = Item->GetSkinItemData().SkeletalMesh;
		PreviewMesh();
	}
	else
	{
		if (SaveGameInstance->MoneyAmount >= Item->GetSkinItemData().Cost)
		{
			Item->SetIsPurchased(true);
			SaveGameInstance->Inventory.Add(Item->GetSkinItemData().ItemName, Item->IsPurchased());
			SaveGameInstance->MoneyAmount -= Item->GetSkinItemData().Cost;

			UpdateButton();

			AIRUIPlayerController* PlayerController = Cast<AIRUIPlayerController>(GetWorld()->GetFirstPlayerController());
			if (PlayerController)
			{
				PlayerController->UpdateShopMoney(SaveGameInstance->MoneyAmount);
			}
		}
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
}

void UIRListShopItemWidget::PreviewMesh()
{
	AIRUIPlayerController* PlayerController = Cast<AIRUIPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		PlayerController->PreviewSkinItem(Item->GetSkinItemData().SkeletalMesh);
	}
}
