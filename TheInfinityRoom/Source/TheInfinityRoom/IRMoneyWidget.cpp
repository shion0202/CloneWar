// Fill out your copyright notice in the Description page of Project Settings.

#include "IRMoneyWidget.h"
#include "Components/TextBlock.h"

void UIRMoneyWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UIRMoneyWidget::UpdateMoney(int32 CurrentMoneyAmount)
{
	TXT_MoneyAmount->SetText(FText::FromString(FString::FromInt(CurrentMoneyAmount)));
}
