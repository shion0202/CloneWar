// Fill out your copyright notice in the Description page of Project Settings.

#include "IRTitleWidget.h"
#include "Components/Button.h"
#include "Animation/WidgetAnimation.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "IRUIPlayerController.h"

UIRTitleWidget::UIRTitleWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<USoundWave> ClickSoundWaveRef(TEXT(
		"/Script/Engine.SoundWave'/Game/Sounds/Sfx_SystemSound_12_Cut.Sfx_SystemSound_12_Cut'"));
	if (ClickSoundWaveRef.Object)
	{
		ClickSoundWave = ClickSoundWaveRef.Object;
	}
}

void UIRTitleWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (nullptr != BTN_SinglePlay)
	{
		BTN_SinglePlay->OnClicked.AddDynamic(this, &UIRTitleWidget::OnSinglePlayClicked);
	}

	//if (nullptr != BTN_MultiPlay)
	//{
	//	BTN_MultiPlay->OnClicked.AddDynamic(this, &UIRTitleWidget::OnMultiPlayClicked);
	//}

	if (nullptr != BTN_Shop)
	{
		BTN_Shop->OnClicked.AddDynamic(this, &UIRTitleWidget::OnShopClicked);
	}

	if (nullptr != BTN_Setting)
	{
		BTN_Setting->OnClicked.AddDynamic(this, &UIRTitleWidget::OnSettingClicked);
	}

	if (nullptr != BTN_Exit)
	{
		BTN_Exit->OnClicked.AddDynamic(this, &UIRTitleWidget::OnExitClicked);
	}

	if (nullptr != BTN_Help)
	{
		BTN_Help->OnClicked.AddDynamic(this, &UIRTitleWidget::OnHelpClicked);
	}

	if (nullptr != BTN_License)
	{
		BTN_License->OnClicked.AddDynamic(this, &UIRTitleWidget::OnLicenseClicked);
	}

	PlayAnimation(Slide);
}

void UIRTitleWidget::OnSinglePlayClicked()
{
	UGameplayStatics::PlaySound2D(this, ClickSoundWave);
	DisableButtons();

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UIRTitleWidget::SinglePlay, 0.5f, false);
}

void UIRTitleWidget::OnMultiPlayClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Multi Play"));
}

void UIRTitleWidget::OnShopClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Shop"));
}

void UIRTitleWidget::OnSettingClicked()
{
	AIRUIPlayerController* PlayerController = Cast<AIRUIPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		PlayerController->OnDisplayWidget(EWidgetType::Setting);
	}
}

void UIRTitleWidget::OnExitClicked()
{
	UGameplayStatics::PlaySound2D(this, ClickSoundWave);
	DisableButtons();

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UIRTitleWidget::Exit, 0.5f, false);
}

void UIRTitleWidget::OnHelpClicked()
{
	AIRUIPlayerController* PlayerController = Cast<AIRUIPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		PlayerController->OnDisplayWidget(EWidgetType::Help);
	}
}

void UIRTitleWidget::OnLicenseClicked()
{
	AIRUIPlayerController* PlayerController = Cast<AIRUIPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		PlayerController->OnDisplayWidget(EWidgetType::License);
	}
}

void UIRTitleWidget::SinglePlay()
{
	UGameplayStatics::OpenLevel(this, TEXT("MainMap"));
}

void UIRTitleWidget::Exit()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, true);
}

void UIRTitleWidget::DisableButtons()
{
	BTN_SinglePlay->SetIsEnabled(false);
	BTN_Shop->SetIsEnabled(false);
	BTN_Setting->SetIsEnabled(false);
	BTN_Exit->SetIsEnabled(false);
	BTN_Help->SetIsEnabled(false);
	BTN_License->SetIsEnabled(false);
}
