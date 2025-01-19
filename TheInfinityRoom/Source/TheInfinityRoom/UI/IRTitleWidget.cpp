// Fill out your copyright notice in the Description page of Project Settings.

#include "IRTitleWidget.h"
#include "Components/Button.h"
#include "Animation/WidgetAnimation.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Game/IRGameInstance.h"
#include "Player/IRUIPlayerController.h"

void UIRTitleWidget::SetIsPlayAnim(bool IsPlay)
{
	bIsPlayAnim = IsPlay;
}

void UIRTitleWidget::OnTitleAnimComplete()
{
	BTN_SinglePlay->SetVisibility(ESlateVisibility::Visible);
	BTN_Shop->SetVisibility(ESlateVisibility::Visible);
	BTN_Setting->SetVisibility(ESlateVisibility::Visible);
	BTN_Exit->SetVisibility(ESlateVisibility::Visible);
	BTN_Ranking->SetVisibility(ESlateVisibility::Visible);
	BTN_Help->SetVisibility(ESlateVisibility::Visible);
	BTN_License->SetVisibility(ESlateVisibility::Visible);
}

UIRTitleWidget::UIRTitleWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<USoundWave> ClickSoundWaveRef(TEXT(
		"/Script/Engine.SoundWave'/Game/Sounds/Sfx_SystemSound_12_Cut.Sfx_SystemSound_12_Cut'"));
	if (ClickSoundWaveRef.Object)
	{
		ClickSoundWave = ClickSoundWaveRef.Object;
	}

	bIsPlayAnim = true;
}

void UIRTitleWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (nullptr != BTN_SinglePlay)
	{
		BTN_SinglePlay->OnClicked.AddUniqueDynamic(this, &UIRTitleWidget::OnSinglePlayClicked);
	}

	//if (nullptr != BTN_MultiPlay)
	//{
	//	BTN_MultiPlay->OnClicked.AddDynamic(this, &UIRTitleWidget::OnMultiPlayClicked);
	//}

	if (nullptr != BTN_Shop)
	{
		BTN_Shop->OnClicked.AddUniqueDynamic(this, &UIRTitleWidget::OnShopClicked);
	}

	if (nullptr != BTN_Setting)
	{
		BTN_Setting->OnClicked.AddUniqueDynamic(this, &UIRTitleWidget::OnSettingClicked);
	}

	if (nullptr != BTN_Exit)
	{
		BTN_Exit->OnClicked.AddUniqueDynamic(this, &UIRTitleWidget::OnExitClicked);
	}

	if (nullptr != BTN_Ranking)
	{
		BTN_Ranking->OnClicked.AddUniqueDynamic(this, &UIRTitleWidget::OnRankingClicked);
	}

	if (nullptr != BTN_Help)
	{
		BTN_Help->OnClicked.AddUniqueDynamic(this, &UIRTitleWidget::OnHelpClicked);
	}

	if (nullptr != BTN_License)
	{
		BTN_License->OnClicked.AddUniqueDynamic(this, &UIRTitleWidget::OnLicenseClicked);
	}

	if (bIsPlayAnim)
	{
		PlayAnimation(Slide);
	}

	UIRGameInstance* GI = Cast<UIRGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI)
	{
		GI->DownloadScores();
	}
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
	AIRUIPlayerController* PlayerController = Cast<AIRUIPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		PlayerController->DisplayShopWidget();
	}
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

void UIRTitleWidget::OnRankingClicked()
{
	AIRUIPlayerController* PlayerController = Cast<AIRUIPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		PlayerController->OnDisplayWidget(EWidgetType::Ranking);
	}
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
	BTN_SinglePlay->SetVisibility(ESlateVisibility::HitTestInvisible);
	BTN_Shop->SetVisibility(ESlateVisibility::HitTestInvisible);
	BTN_Setting->SetVisibility(ESlateVisibility::HitTestInvisible);
	BTN_Exit->SetVisibility(ESlateVisibility::HitTestInvisible);
	BTN_Ranking->SetVisibility(ESlateVisibility::HitTestInvisible);
	BTN_Help->SetVisibility(ESlateVisibility::HitTestInvisible);
	BTN_License->SetVisibility(ESlateVisibility::HitTestInvisible);
}
