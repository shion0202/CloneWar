// Fill out your copyright notice in the Description page of Project Settings.

#include "IRHUDWidget.h"
#include "Interface/IRCharacterHUDInterface.h"
#include "IRStageLevelWidget.h"
#include "IRCharacterStatWidget.h"

UIRHUDWidget::UIRHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) { }

void UIRHUDWidget::UpdateStat(const FIRCharacterStat& TotalStat)
{
	CharacterStat->UpdateStat(TotalStat);
}

void UIRHUDWidget::UpdateStageLevel(int32 CurrentStageLevel)
{
	StageLevel->UpdateStageLevel(CurrentStageLevel);
}

void UIRHUDWidget::UpdateNumOfEnemies(int32 InDestroyEnemyCount, int32 InTargetEnemyCount)
{
	StageLevel->UpdateNumOfEnemies(InDestroyEnemyCount, InTargetEnemyCount);
}

void UIRHUDWidget::UpdateObjective(bool IsReady)
{
	StageLevel->UpdateObjective(IsReady);
}

void UIRHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	StageLevel = Cast<UIRStageLevelWidget>(GetWidgetFromName(TEXT("WidgetStageLevel")));
	CharacterStat = Cast<UIRCharacterStatWidget>(GetWidgetFromName(TEXT("WidgetCharacterStat")));

	IIRCharacterHUDInterface* HUDPawn = Cast<IIRCharacterHUDInterface>(GetOwningPlayerPawn());
	if (HUDPawn)
	{
		HUDPawn->SetupHUDWidget(this);
	}
}
