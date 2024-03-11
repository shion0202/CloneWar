// Fill out your copyright notice in the Description page of Project Settings.

#include "IRShopWidget.h"
#include "Components/Button.h"
#include "IRMoneyWidget.h"
#include "IRShopListWidget.h"
#include "IRUIPlayerController.h"
#include "IRGameModeBase.h"
#include "IRSaveGame.h"

UIRShopWidget::UIRShopWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) { }

void UIRShopWidget::UpdateMoneyAmount(int32 CurrentMoneyAmount)
{
	Money->UpdateMoney(CurrentMoneyAmount);
}

void UIRShopWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Money = Cast<UIRMoneyWidget>(GetWidgetFromName(TEXT("WBP_Money")));
	ShopList = Cast<UIRShopListWidget>(GetWidgetFromName(TEXT("WBP_ShopList")));

	if (nullptr != BTN_ReturnToTitle)
	{
		BTN_ReturnToTitle->OnClicked.AddUniqueDynamic(this, &UIRShopWidget::OnReturnClicked);
	}

	AIRGameModeBase* GameModeBase = Cast<AIRGameModeBase>(GetWorld()->GetAuthGameMode());
	UIRSaveGame* SaveGameInstance = GameModeBase->GetSaveGameInstance();
	UpdateMoneyAmount(SaveGameInstance->MoneyAmount);
}

void UIRShopWidget::OnReturnClicked()
{
	AIRUIPlayerController* PlayerController = Cast<AIRUIPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		AIRGameModeBase* GameModeBase = Cast<AIRGameModeBase>(GetWorld()->GetAuthGameMode());
		UIRSaveGame* SaveGameInstance = GameModeBase->GetSaveGameInstance();
		PlayerController->PreviewSkinItem(SaveGameInstance->EquipedMesh);

		PlayerController->DisplayTitleWidget();
	}
}
