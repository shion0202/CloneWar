// Fill out your copyright notice in the Description page of Project Settings.

#include "IRLicenseWidget.h"
#include "Components/Button.h"

void UIRLicenseWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (nullptr != BTN_Close)
	{
		BTN_Close->OnClicked.AddDynamic(this, &UIRLicenseWidget::OnCloseClick);
	}
}

void UIRLicenseWidget::OnCloseClick()
{
	RemoveFromParent();
}
