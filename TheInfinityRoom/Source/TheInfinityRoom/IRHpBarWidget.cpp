// Fill out your copyright notice in the Description page of Project Settings.

#include "IRHpBarWidget.h"
#include "Components/ProgressBar.h"
#include "IRCharacterWidgetInterface.h"

void UIRHpBarWidget::NativeConstruct()
{
	IIRCharacterWidgetInterface* CharacterWidget = Cast<IIRCharacterWidgetInterface>(Owner);
	if (CharacterWidget)
	{
		CharacterWidget->SetupCharacterWidget(this);
	}
}

void UIRHpBarWidget::UpdateHp(float ratio)
{
	PB_HpBar->SetPercent(ratio);
}
