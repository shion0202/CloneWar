// Fill out your copyright notice in the Description page of Project Settings.

#include "IRRuleWidget.h"
#include "Components/Button.h"
#include "Player/IRUIPlayerController.h"

void UIRRuleWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (nullptr != BTN_Close)
	{
		BTN_Close->OnClicked.AddDynamic(this, &UIRRuleWidget::OnCloseClick);
	}

	if (nullptr != BTN_Left)
	{
		BTN_Left->OnClicked.AddDynamic(this, &UIRRuleWidget::OnLeftClick);
	}

	if (nullptr != BTN_Right)
	{
		BTN_Right->OnClicked.AddDynamic(this, &UIRRuleWidget::OnRightClick);
	}
}

void UIRRuleWidget::OnCloseClick()
{
	RemoveFromParent();
}

void UIRRuleWidget::OnLeftClick()
{
	RemoveFromParent();

	AIRUIPlayerController* PlayerController = Cast<AIRUIPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		PlayerController->OnDisplayWidget(EWidgetType::Help);
	}
}

void UIRRuleWidget::OnRightClick()
{
	RemoveFromParent();

	AIRUIPlayerController* PlayerController = Cast<AIRUIPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		PlayerController->OnDisplayWidget(EWidgetType::Tutorial);
	}
}
