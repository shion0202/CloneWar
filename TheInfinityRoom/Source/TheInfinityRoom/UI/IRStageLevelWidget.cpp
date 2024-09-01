// Fill out your copyright notice in the Description page of Project Settings.

#include "IRStageLevelWidget.h"
#include "Components/TextBlock.h"

void UIRStageLevelWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UIRStageLevelWidget::UpdateStageLevel(int32 CurrentStageLevel)
{
	TXT_StageLevel->SetText(FText::FromString(FString::FromInt(CurrentStageLevel)));
}
