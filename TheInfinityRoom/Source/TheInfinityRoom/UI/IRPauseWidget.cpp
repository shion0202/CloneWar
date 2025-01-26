// Fill out your copyright notice in the Description page of Project Settings.

#include "IRPauseWidget.h"
#include "Player/IRPlayerController.h"
#include "Components/Button.h"
#include "Interface/IRGameOverInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

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

	UGameplayStatics::PlaySound2D(this, PauseSoundWave);
}

UIRPauseWidget::UIRPauseWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<USoundWave> PauseSoundWaveRef(TEXT(
		"/Script/Engine.SoundWave'/Game/ProgressiveUI/Sounds/SW_ProgressiveUI_Notify.SW_ProgressiveUI_Notify'"));
	if (PauseSoundWaveRef.Object)
	{
		PauseSoundWave = PauseSoundWaveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundWave> ClickSoundWaveRef(TEXT(
		"/Script/Engine.SoundWave'/Game/ProgressiveUI/Sounds/SW_ProgressiveUI_Click.SW_ProgressiveUI_Click'"));
	if (ClickSoundWaveRef.Object)
	{
		ClickSoundWave = ClickSoundWaveRef.Object;
	}
}

void UIRPauseWidget::OnResumeClicked()
{
	AIRPlayerController* PlayerController = Cast<AIRPlayerController>(GetOwningPlayer());
	if (PlayerController)
	{
		PlayerController->SetPause(false);

		FInputModeGameOnly InputMode;
		PlayerController->SetInputMode(InputMode);
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
	Retry();
}

void UIRPauseWidget::OnReturnToTitleClicked()
{
	ReturnToTitle();
}

void UIRPauseWidget::Retry()
{
	AIRPlayerController* PlayerController = Cast<AIRPlayerController>(GetOwningPlayer());
	if (PlayerController)
	{
		PlayerController->RestartLevel();
	}
}

void UIRPauseWidget::ReturnToTitle()
{
	UGameplayStatics::OpenLevel(this, TEXT("TitleMap"));
}
