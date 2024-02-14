// Fill out your copyright notice in the Description page of Project Settings.

#include "IRPauseWidget.h"
#include "IRPlayerController.h"
#include "Components/Button.h"
#include "IRGameOverInterface.h"
#include "Kismet/GameplayStatics.h"

void UIRPauseWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (nullptr != BTN_Resume)
	{
		BTN_Resume->OnClicked.AddDynamic(this, &UIRPauseWidget::OnResumeClicked);
	}

	if (nullptr != BTN_Retry)
	{
		BTN_Retry->OnClicked.AddDynamic(this, &UIRPauseWidget::OnRetryClicked);
	}

	if (nullptr != BTN_ReturnToTitle)
	{
		BTN_ReturnToTitle->OnClicked.AddDynamic(this, &UIRPauseWidget::OnReturnToTitleClicked);
	}
}

void UIRPauseWidget::OnResumeClicked()
{
	AIRPlayerController* PlayerController = Cast<AIRPlayerController>(GetOwningPlayer());
	if (PlayerController)
	{
		PlayerController->SetPause(false);
	}

	IIRGameOverInterface* PauseInterface = Cast<IIRGameOverInterface>(GetOwningPlayer()->GetPawn());
	if (PauseInterface)
	{
		PauseInterface->ChangeCharacterControl();
	}

	RemoveFromParent();
}

void UIRPauseWidget::OnRetryClicked()
{
	AIRPlayerController* PlayerController = Cast<AIRPlayerController>(GetOwningPlayer());
	PlayerController->RestartLevel();
}

void UIRPauseWidget::OnReturnToTitleClicked()
{
	UGameplayStatics::OpenLevel(this, TEXT("TitleMap"));
}
