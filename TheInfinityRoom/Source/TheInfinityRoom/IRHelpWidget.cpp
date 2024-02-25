// Fill out your copyright notice in the Description page of Project Settings.

#include "IRHelpWidget.h"
#include "Components/Button.h"
#include "IRUIPlayerController.h"

void UIRHelpWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (nullptr != BTN_Close)
	{
		BTN_Close->OnClicked.AddDynamic(this, &UIRHelpWidget::OnCloseClick);
	}

	if (nullptr != BTN_Right)
	{
		BTN_Right->OnClicked.AddDynamic(this, &UIRHelpWidget::OnRightClick);
	}
}

void UIRHelpWidget::OnCloseClick()
{
	RemoveFromParent();
}

void UIRHelpWidget::OnRightClick()
{
	RemoveFromParent();

	AIRUIPlayerController* PlayerController = Cast<AIRUIPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		PlayerController->OnDisplayWidget(EWidgetType::Rule);
	}
}
