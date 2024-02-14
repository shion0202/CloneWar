// Fill out your copyright notice in the Description page of Project Settings.

#include "IRHelpWidget.h"
#include "Components/Button.h"

void UIRHelpWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (nullptr != BTN_Close)
	{
		BTN_Close->OnClicked.AddDynamic(this, &UIRHelpWidget::OnCloseClick);
	}
}

void UIRHelpWidget::OnCloseClick()
{
	RemoveFromParent();
}
