// Fill out your copyright notice in the Description page of Project Settings.

#include "IRPlayerController.h"
#include "IRHUDWidget.h"
#include "IRGameOverWidget.h"
#include "IRPauseWidget.h"
#include "IRCharacterPlayer.h"

AIRPlayerController::AIRPlayerController()
{
	static ConstructorHelpers::FClassFinder<UIRHUDWidget> HUDWidgetRef(TEXT(
		"/Game/UI/WBP_HUD.WBP_HUD_C"));
	if (HUDWidgetRef.Class)
	{
		HUDWidgetClass = HUDWidgetRef.Class;
	}

	static ConstructorHelpers::FClassFinder<UIRGameOverWidget> GameOverWidgetRef(TEXT(
		"/Game/UI/WBP_GameOver.WBP_GameOver_C"));
	if (GameOverWidgetRef.Class)
	{
		GameOverWidgetClass = GameOverWidgetRef.Class;
	}

	static ConstructorHelpers::FClassFinder<UIRPauseWidget> PauseWidgetRef(TEXT(
		"/Game/UI/WBP_Pause.WBP_Pause_C"));
	if (PauseWidgetRef.Class)
	{
		PauseWidgetClass = PauseWidgetRef.Class;
	}
}

void AIRPlayerController::GameOver()
{
	GetWorld()->GetTimerManager().SetTimer(WaitingTimeHandle, this, &AIRPlayerController::DisplayGameOverWidget, 2.f, false);
}

void AIRPlayerController::OnGamePause()
{
	AIRCharacterPlayer* CharacterPlayer = Cast<AIRCharacterPlayer>(GetPawn());
	if (CharacterPlayer)
	{
		if (CharacterPlayer->GetCurrentControlType() == ECharacterControlType::Default)
		{
			FInputModeGameAndUI InputMode;
			SetInputMode(InputMode);

			PauseWidget = CreateWidget<UIRPauseWidget>(this, PauseWidgetClass);
			if (PauseWidget)
			{
				PauseWidget->AddToViewport();
			}

			IIRGameOverInterface* PauseInterface = Cast<IIRGameOverInterface>(GetPawn());
			if (PauseInterface)
			{
				PauseInterface->ChangeCharacterControl();
			}

			SetPause(true);
		}
		else if (CharacterPlayer->GetCurrentControlType() == ECharacterControlType::UI)
		{
			PauseWidget->OnResumeClicked();
		}
	}
}

void AIRPlayerController::BeginPlay()
{
	Super::BeginPlay();

	HUDWidget = CreateWidget<UIRHUDWidget>(this, HUDWidgetClass);
	if (HUDWidget)
	{
		HUDWidget->AddToViewport();
	}

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
}

void AIRPlayerController::DisplayGameOverWidget()
{
	FInputModeUIOnly InputMode;
	SetInputMode(InputMode);

	GameOverWidget = CreateWidget<UIRGameOverWidget>(this, GameOverWidgetClass);
	if (GameOverWidget)
	{
		GameOverWidget->AddToViewport();
	}
}
