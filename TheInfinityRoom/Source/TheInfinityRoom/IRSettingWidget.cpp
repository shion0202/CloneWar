// Fill out your copyright notice in the Description page of Project Settings.

#include "IRSettingWidget.h"
#include "Components/Button.h"
#include "GameFramework/GameUserSettings.h"
#include "IRUIPlayerController.h"
#include "IRSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "IRGameModeBase.h"

void UIRSettingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (nullptr != BTN_Apply)
	{
		BTN_Apply->OnClicked.AddDynamic(this, &UIRSettingWidget::OnApplyClick);
	}

	if (nullptr != BTN_Close)
	{
		BTN_Close->OnClicked.AddDynamic(this, &UIRSettingWidget::OnCloseClick);
	}
}

void UIRSettingWidget::OnApplyClick()
{
	//UGameUserSettings* Setting = GEngine->GetGameUserSettings();
	//if (Setting)
	//{
	//	Setting->ApplySettings(true);
	//}

	//AIRGameModeBase* GameModeBase = Cast<AIRGameModeBase>(GetWorld()->GetAuthGameMode());
	//GameModeBase->SetVolumeData(BGMValue, SEValue);

	//RemoveFromParent();
}

void UIRSettingWidget::OnCloseClick()
{
	UGameUserSettings* Setting = GEngine->GetGameUserSettings();
	if (Setting)
	{
		Setting->LoadSettings();
	}

	AIRUIPlayerController* PlayerController = Cast<AIRUIPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		PlayerController->SetVolumeToDefault();
	}

	RemoveFromParent();
}
