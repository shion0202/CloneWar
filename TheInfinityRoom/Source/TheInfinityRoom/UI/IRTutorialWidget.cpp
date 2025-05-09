// Fill out your copyright notice in the Description page of Project Settings.

#include "IRTutorialWidget.h"
#include "Components/Button.h"
#include "Player/IRUIPlayerController.h"

void UIRTutorialWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (nullptr != BTN_Close)
	{
		BTN_Close->OnClicked.AddDynamic(this, &UIRTutorialWidget::OnCloseClick);
	}

	if (nullptr != BTN_Left)
	{
		BTN_Left->OnClicked.AddDynamic(this, &UIRTutorialWidget::OnLeftClick);
	}
}

void UIRTutorialWidget::OnCloseClick()
{
	AIRUIPlayerController* PlayerController = Cast<AIRUIPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		PlayerController->EnableButtons();
	}

	RemoveFromParent();
}

void UIRTutorialWidget::OnLeftClick()
{
	RemoveFromParent();

	AIRUIPlayerController* PlayerController = Cast<AIRUIPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		PlayerController->OnDisplayWidget(EWidgetType::Rule);
	}
}
