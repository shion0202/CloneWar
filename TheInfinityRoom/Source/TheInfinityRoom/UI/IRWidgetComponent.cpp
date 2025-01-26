// Fill out your copyright notice in the Description page of Project Settings.

#include "IRWidgetComponent.h"
#include "IRUserWidget.h"

void UIRWidgetComponent::InitWidget()
{
	Super::InitWidget();

	UIRUserWidget* UserWidget = Cast<UIRUserWidget>(GetWidget());
	if (UserWidget)
	{
		UserWidget->SetOwner(GetOwner());
	}
}