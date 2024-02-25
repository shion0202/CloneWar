// Fill out your copyright notice in the Description page of Project Settings.

#include "IRUIPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "IRTitleWidget.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "IRSaveGame.h"
#include "Sound/SoundMix.h"
#include "Sound/SoundClass.h"
#include "IRGameModeBase.h"

AIRUIPlayerController::AIRUIPlayerController()
{
	static ConstructorHelpers::FClassFinder<UIRTitleWidget> TitleWidgetRef(TEXT(
		"/Game/UI/WBP_Title.WBP_Title_C"));
	if (TitleWidgetRef.Class)
	{
		TitleWidgetClass = TitleWidgetRef.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> SettingWidgetRef(TEXT(
		"/Game/UI/WBP_Setting.WBP_Setting_C"));
	if (SettingWidgetRef.Class)
	{
		SettingWidgetClass = SettingWidgetRef.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> HelpWidgetRef(TEXT(
		"/Game/UI/WBP_Help.WBP_Help_C"));
	if (HelpWidgetRef.Class)
	{
		HelpWidgetClass = HelpWidgetRef.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> RuleWidgetRef(TEXT(
		"/Game/UI/WBP_Rule.WBP_Rule_C"));
	if (RuleWidgetRef.Class)
	{
		RuleWidgetClass = RuleWidgetRef.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> TutorialWidgetRef(TEXT(
		"/Game/UI/WBP_Tutorial.WBP_Tutorial_C"));
	if (TutorialWidgetRef.Class)
	{
		TutorialWidgetClass = TutorialWidgetRef.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> LicenseWidgetRef(TEXT(
		"/Game/UI/WBP_License.WBP_License_C"));
	if (LicenseWidgetRef.Class)
	{
		LicenseWidgetClass = LicenseWidgetRef.Class;
	}

	DisplayWidgetActions.Add(FDisplayWidgetDelegateWrapper(FOnDisplayWidgetDelegate::CreateUObject(
		this, &AIRUIPlayerController::DisplaySettingWidget)));
	DisplayWidgetActions.Add(FDisplayWidgetDelegateWrapper(FOnDisplayWidgetDelegate::CreateUObject(
		this, &AIRUIPlayerController::DisplayHelpWidget)));
	DisplayWidgetActions.Add(FDisplayWidgetDelegateWrapper(FOnDisplayWidgetDelegate::CreateUObject(
		this, &AIRUIPlayerController::DisplayRuleWidget)));
	DisplayWidgetActions.Add(FDisplayWidgetDelegateWrapper(FOnDisplayWidgetDelegate::CreateUObject(
		this, &AIRUIPlayerController::DisplayTutorialWidget)));
	DisplayWidgetActions.Add(FDisplayWidgetDelegateWrapper(FOnDisplayWidgetDelegate::CreateUObject(
		this, &AIRUIPlayerController::DisplayLicenseWidget)));

	static ConstructorHelpers::FObjectFinder<USoundCue> BGMSoundCueRef(TEXT(
		"/Script/Engine.SoundCue'/Game/Sounds/BGM_60_Cue.BGM_60_Cue'"));
	if (BGMSoundCueRef.Object)
	{
		BGMSoundCue = BGMSoundCueRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundMix> MasterSoundMixRef(TEXT(
		"/Script/Engine.SoundMix'/Game/Sounds/SM_Master.SM_Master'"));
	if (MasterSoundMixRef.Object)
	{
		MasterSoundMix = MasterSoundMixRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundClass> BGMSoundClassRef(TEXT(
		"/Script/Engine.SoundClass'/Game/Sounds/SC_BGM.SC_BGM'"));
	if (BGMSoundClassRef.Object)
	{
		BGMSoundClass = BGMSoundClassRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundClass> SESoundClassRef(TEXT(
		"/Script/Engine.SoundClass'/Game/Sounds/SC_SE.SC_SE'"));
	if (SESoundClassRef.Object)
	{
		SESoundClass = SESoundClassRef.Object;
	}

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->bAutoActivate = false;
	AudioComponent->SetupAttachment(RootComponent);
}

void AIRUIPlayerController::OnDisplayWidget(EWidgetType InType)
{
	DisplayWidgetActions[(uint8)InType].DisplayWidgetDelegate.ExecuteIfBound(InType);

	if (UIWidget)
	{
		UIWidget->AddToViewport();
	}
}

void AIRUIPlayerController::SetVolumeToDefault()
{
	AIRGameModeBase* GameModeBase = Cast<AIRGameModeBase>(GetWorld()->GetAuthGameMode());
	UIRSaveGame* SaveGameInstance = GameModeBase->GetSaveGameInstance();
	UGameplayStatics::SetSoundMixClassOverride(GetWorld(),
		MasterSoundMix, BGMSoundClass, SaveGameInstance->BGMVolumeValue, 1.f, 0.f);
	UGameplayStatics::SetSoundMixClassOverride(GetWorld(),
		MasterSoundMix, SESoundClass, SaveGameInstance->SEVolumeValue, 1.f, 0.f);
	UGameplayStatics::PushSoundMixModifier(GetWorld(), MasterSoundMix);
}

void AIRUIPlayerController::SetLanguage(FString InLanguage)
{
	FInternationalization::Get().SetCurrentCulture(InLanguage);

	AIRGameModeBase* GameModeBase = Cast<AIRGameModeBase>(GetWorld()->GetAuthGameMode());
	GameModeBase->SetLanguageData(InLanguage);
}

void AIRUIPlayerController::BeginPlay()
{
	Super::BeginPlay();

	TitleWidget = CreateWidget<UIRTitleWidget>(this, TitleWidgetClass);
	if (TitleWidget)
	{
		TitleWidget->AddToViewport();
	}

	bShowMouseCursor = true;

	SetVolumeToDefault();

	AIRGameModeBase* GameModeBase = Cast<AIRGameModeBase>(GetWorld()->GetAuthGameMode());
	UIRSaveGame* SaveGameInstance = GameModeBase->GetSaveGameInstance();
	FInternationalization::Get().SetCurrentCulture(SaveGameInstance->CurrentLanguage);

	AudioComponent->SetSound(Cast<USoundBase>(BGMSoundCue));
	AudioComponent->Play();
}

void AIRUIPlayerController::DisplaySettingWidget(EWidgetType InType)
{
	UIWidget = CreateWidget<UUserWidget>(this, SettingWidgetClass);
}

void AIRUIPlayerController::DisplayHelpWidget(EWidgetType InType)
{
	UIWidget = CreateWidget<UUserWidget>(this, HelpWidgetClass);
}

void AIRUIPlayerController::DisplayRuleWidget(EWidgetType InType)
{
	UIWidget = CreateWidget<UUserWidget>(this, RuleWidgetClass);
}

void AIRUIPlayerController::DisplayTutorialWidget(EWidgetType InType)
{
	UIWidget = CreateWidget<UUserWidget>(this, TutorialWidgetClass);
}

void AIRUIPlayerController::DisplayLicenseWidget(EWidgetType InType)
{
	UIWidget = CreateWidget<UUserWidget>(this, LicenseWidgetClass);
}
