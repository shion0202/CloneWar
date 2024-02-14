// Fill out your copyright notice in the Description page of Project Settings.

#include "IRGameOverWidget.h"
#include "Components/Button.h"
#include "IRPlayerController.h"
#include "IRGameInterface.h"
#include "GameFramework/GameModeBase.h"
#include "Components/TextBlock.h"
#include "Animation/WidgetAnimation.h"
#include "Kismet/GameplayStatics.h"

void UIRGameOverWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (nullptr != BTN_Retry)
	{
		BTN_Retry->OnClicked.AddDynamic(this, &UIRGameOverWidget::OnRetryClicked);
	}
	
	if (nullptr != BTN_ReturnToTitle)
	{
		BTN_ReturnToTitle->OnClicked.AddDynamic(this, &UIRGameOverWidget::OnReturnToTitleClicked);
	}

	IIRGameInterface* GameInterface = Cast<IIRGameInterface>(GetWorld()->GetAuthGameMode());
	if (GameInterface)
	{
		TXT_StageLevel->SetText(FText::FromString(FString::FromInt(GameInterface->GetStageLevel())));
	}

	PlayAnimation(Fade);
}

void UIRGameOverWidget::OnRetryClicked()
{
	AIRPlayerController* PlayerController = Cast<AIRPlayerController>(GetOwningPlayer());
	PlayerController->RestartLevel();
}

void UIRGameOverWidget::OnReturnToTitleClicked()
{
	UGameplayStatics::OpenLevel(this, TEXT("TitleMap"));
}
