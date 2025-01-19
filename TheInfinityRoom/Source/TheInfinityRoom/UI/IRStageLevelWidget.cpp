// Fill out your copyright notice in the Description page of Project Settings.

#include "IRStageLevelWidget.h"
#include "Components/TextBlock.h"
#include "Game/IRGameModeBase.h"
#include "Player/IRSaveGame.h"

void UIRStageLevelWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UIRStageLevelWidget::UpdateStageLevel(int32 CurrentStageLevel)
{
	TXT_StageLevel->SetText(FText::FromString(FString::FromInt(CurrentStageLevel)));
}

void UIRStageLevelWidget::UpdateNumOfEnemies(int32 InDestroyEnemyCount, int32 InTargetEnemyCount)
{
	TXT_NumOfEnemies->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), InDestroyEnemyCount, InTargetEnemyCount)));
}

void UIRStageLevelWidget::UpdateObjective(bool IsReady)
{
	AIRGameModeBase* GameModeBase = Cast<AIRGameModeBase>(GetWorld()->GetAuthGameMode());
	UIRSaveGame* SaveGameInstance = GameModeBase->GetSaveGameInstance();

	if (IsReady)
	{
		if (SaveGameInstance && SaveGameInstance->CurrentLanguage.Equals(TEXT("ko")))
		{
			TXT_Objective->SetText(FText::FromString(TEXT("모든 적을 쓰러뜨리세요.")));
		}
		else
		{
			TXT_Objective->SetText(FText::FromString("Take down all enemies."));
		}
	}
	else
	{
		if (SaveGameInstance && SaveGameInstance->CurrentLanguage.Equals(TEXT("ko")))
		{
			TXT_Objective->SetText(FText::FromString(TEXT("보상을 선택하세요.")));
		}
		else
		{
			TXT_Objective->SetText(FText::FromString("Choose a reward."));
		}
	}
}
