// Fill out your copyright notice in the Description page of Project Settings.

#include "IRHpBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Interface/IRCharacterWidgetInterface.h"

void UIRHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	IIRCharacterWidgetInterface* CharacterWidget = Cast<IIRCharacterWidgetInterface>(Owner);
	if (CharacterWidget)
	{
		CharacterWidget->SetupCharacterWidget(this);
	}
}

void UIRHpBarWidget::UpdateHp(float InMaxHp, float InCurrentHp)
{
	PB_HpBar->SetPercent(InCurrentHp / InMaxHp);
	TXT_HpRatio->SetText(FText::FromString(*FString::Printf(TEXT("%.0f / %.0f"), InCurrentHp, InMaxHp)));
}
