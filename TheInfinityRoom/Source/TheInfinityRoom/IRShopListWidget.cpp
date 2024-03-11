// Fill out your copyright notice in the Description page of Project Settings.

#include "IRShopListWidget.h"
#include "Components/Button.h"
#include "Components/ListView.h"
#include "IRSkinItem.h"
#include "IRSkinItemObject.h"
#include "IRGameSingleton.h"
#include "IRGameModeBase.h"
#include "IRSaveGame.h"

void UIRShopListWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (nullptr != BTN_SkinItem)
	{
		BTN_SkinItem->OnClicked.AddUniqueDynamic(this, &UIRShopListWidget::OnClickSkinItem);
	}

	AIRGameModeBase* GameModeBase = Cast<AIRGameModeBase>(GetWorld()->GetAuthGameMode());
	UIRSaveGame* SaveGameInstance = GameModeBase->GetSaveGameInstance();

	LV_ShopItemList->ClearListItems();
	TArray<FIRSkinItem> SkinItemTable = UIRGameSingleton::Get().GetSkinItems();
	for (FIRSkinItem SkinItem : SkinItemTable)
	{
		UIRSkinItemObject* SkinItemObject = NewObject<UIRSkinItemObject>();
		SkinItemObject->SetSkinItemData(SkinItem);

		if (SaveGameInstance->Inventory.Contains(SkinItem.ItemName))
		{
			bool* Value = SaveGameInstance->Inventory.Find(SkinItem.ItemName);
			SkinItemObject->SetIsPurchased(*Value);
		}

		LV_ShopItemList->AddItem(SkinItemObject);
	}
}

void UIRShopListWidget::OnClickSkinItem()
{
	UE_LOG(LogTemp, Warning, TEXT("Skin Item"));
}
