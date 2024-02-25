// Fill out your copyright notice in the Description page of Project Settings.

#include "IRGameOverWidget.h"
#include "Components/Button.h"
#include "IRPlayerController.h"
#include "IRGameInterface.h"
#include "GameFramework/GameModeBase.h"
#include "Components/TextBlock.h"
#include "Animation/WidgetAnimation.h"
#include "Kismet/GameplayStatics.h"

UIRGameOverWidget::UIRGameOverWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<USoundWave> ClickSoundWaveRef(TEXT(
		"/Script/Engine.SoundWave'/Game/Sounds/Sfx_SystemSound_12_Cut.Sfx_SystemSound_12_Cut'"));
	if (ClickSoundWaveRef.Object)
	{
		ClickSoundWave = ClickSoundWaveRef.Object;
	}
}

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
	UGameplayStatics::PlaySound2D(this, ClickSoundWave);
	DisableButtons();

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UIRGameOverWidget::Retry, 0.5f, false);
}

void UIRGameOverWidget::OnReturnToTitleClicked()
{
	UGameplayStatics::PlaySound2D(this, ClickSoundWave);
	DisableButtons();

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UIRGameOverWidget::ReturnToTitle, 0.5f, false);
}

void UIRGameOverWidget::Retry()
{
	AIRPlayerController* PlayerController = Cast<AIRPlayerController>(GetOwningPlayer());
	PlayerController->RestartLevel();
}

void UIRGameOverWidget::ReturnToTitle()
{
	UGameplayStatics::OpenLevel(this, TEXT("TitleMap"));
}

void UIRGameOverWidget::DisableButtons()
{
	BTN_Retry->SetIsEnabled(false);
	BTN_ReturnToTitle->SetIsEnabled(false);
}
