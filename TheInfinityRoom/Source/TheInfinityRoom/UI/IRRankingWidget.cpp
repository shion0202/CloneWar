// Fill out your copyright notice in the Description page of Project Settings.

#include "IRRankingWidget.h"
#include "Components/Button.h"

void UIRRankingWidget::NativeConstruct()
{
	if (nullptr != BTN_Close)
	{
		BTN_Close->OnClicked.AddDynamic(this, &UIRRankingWidget::OnCloseClick);
	}
}

void UIRRankingWidget::OnCloseClick()
{
	RemoveFromParent();
}
