// Fill out your copyright notice in the Description page of Project Settings.

#include "IRUIPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "UI/IRTitleWidget.h"
#include "UI/IRShopWidget.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "IRSaveGame.h"
#include "Sound/SoundMix.h"
#include "Sound/SoundClass.h"
#include "Game/IRGameModeBase.h"
#include "Character/IRCharacterShopPlayer.h"
#include "Camera/CameraComponent.h"
#include "Item/IRShopItemObject.h"
#include "GameFramework/PlayerState.h"

AIRUIPlayerController::AIRUIPlayerController()
{
	static ConstructorHelpers::FClassFinder<UIRTitleWidget> TitleWidgetRef(TEXT(
		"/Game/UI/WBP_Title.WBP_Title_C"));
	if (TitleWidgetRef.Class)
	{
		TitleWidgetClass = TitleWidgetRef.Class;
	}

	static ConstructorHelpers::FClassFinder<UIRShopWidget> ShopWidgetRef(TEXT(
		"/Game/UI/WBP_Shop.WBP_Shop_C"));
	if (ShopWidgetRef.Class)
	{
		ShopWidgetClass = ShopWidgetRef.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> SettingWidgetRef(TEXT(
		"/Game/UI/WBP_Setting.WBP_Setting_C"));
	if (SettingWidgetRef.Class)
	{
		SettingWidgetClass = SettingWidgetRef.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> RankingWidgetRef(TEXT(
		"/Game/UI/WBP_Ranking.WBP_Ranking_C"));
	if (RankingWidgetRef.Class)
	{
		RankingWidgetClass = RankingWidgetRef.Class;
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
		this, &AIRUIPlayerController::DisplayRankingWidget)));
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

	PreviewItemActions.Add(FPreviewItemDelegateWrapper(FOnPreviewItemDelegate::CreateUObject(
		this, &AIRUIPlayerController::PreviewSkinItem)));
	PreviewItemActions.Add(FPreviewItemDelegateWrapper(FOnPreviewItemDelegate::CreateUObject(
		this, &AIRUIPlayerController::PreviewHeadItem)));
	PreviewItemActions.Add(FPreviewItemDelegateWrapper(FOnPreviewItemDelegate::CreateUObject(
		this, &AIRUIPlayerController::PreviewBackItem)));
	PreviewItemActions.Add(FPreviewItemDelegateWrapper(FOnPreviewItemDelegate::CreateUObject(
		this, &AIRUIPlayerController::PreviewEffectItem)));
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

void AIRUIPlayerController::DisplayTitleWidget()
{
	if (TitleWidget && ShopWidget)
	{
		//ShopWidget->RemoveFromViewport();
		ShopWidget->RemoveFromParent();
		SetCameraTransform(false);
		TitleWidget->SetIsPlayAnim(false);
		TitleWidget->AddToViewport();

		//FInputModeUIOnly InputMode;
		//SetInputMode(InputMode);
	}
}

void AIRUIPlayerController::DisplayShopWidget()
{
	if (TitleWidget && ShopWidget)
	{
		//TitleWidget->RemoveFromViewport();
		TitleWidget->RemoveFromParent();
		SetCameraTransform(true);
		ShopWidget->AddToViewport();

		//FInputModeGameAndUI InputMode;
		//SetInputMode(InputMode);
	}
}

void AIRUIPlayerController::PreviewItem(FIRItem ItemData)
{
	PreviewItemActions[(uint8)ItemData.ItemType].PreviewItemDelegate.ExecuteIfBound(ItemData);
}

void AIRUIPlayerController::EquipItems()
{
	AIRGameModeBase* GameModeBase = Cast<AIRGameModeBase>(GetWorld()->GetAuthGameMode());
	AIRCharacterShopPlayer* ShopPlayer = Cast<AIRCharacterShopPlayer>(GetPawn());
	if (GameModeBase && ShopPlayer)
	{
		UIRSaveGame* SaveGameInstance = GameModeBase->GetSaveGameInstance();
		ShopPlayer->GetMesh()->SetSkeletalMesh(SaveGameInstance->EquipedSkin);
		ShopPlayer->SetHeadSocket(SaveGameInstance->HeadSocketName);
		ShopPlayer->EquipHeadItem(*SaveGameInstance->EquipedItems.Find(TEXT("Head")));
		ShopPlayer->EquipBackItem(*SaveGameInstance->EquipedItems.Find(TEXT("Back")));
	}
}

void AIRUIPlayerController::PreviewSkinItem(FIRItem ItemData)
{
	AIRCharacterShopPlayer* ShopPlayer = Cast<AIRCharacterShopPlayer>(GetPawn());
	if (ShopPlayer && ItemData.SkeletalMesh)
	{
		ShopPlayer->GetMesh()->SetSkeletalMesh(ItemData.SkeletalMesh);
		ShopPlayer->SetHeadSocket(ItemData.HeadSocketName);
	}
}

void AIRUIPlayerController::PreviewHeadItem(FIRItem ItemData)
{
	AIRCharacterShopPlayer* ShopPlayer = Cast<AIRCharacterShopPlayer>(GetPawn());
	if (ShopPlayer)
	{
		ShopPlayer->EquipHeadItem(ItemData.StaticMesh);
	}
}

void AIRUIPlayerController::PreviewBackItem(FIRItem ItemData)
{
	AIRCharacterShopPlayer* ShopPlayer = Cast<AIRCharacterShopPlayer>(GetPawn());
	if (ShopPlayer)
	{
		ShopPlayer->EquipBackItem(ItemData.StaticMesh);
	}
}

void AIRUIPlayerController::PreviewEffectItem(FIRItem ItemData)
{
	AIRCharacterShopPlayer* ShopPlayer = Cast<AIRCharacterShopPlayer>(GetPawn());
	if (ShopPlayer)
	{
		ShopPlayer->PlayEffectForPreview(ItemData.Effect);
	}
}

void AIRUIPlayerController::UpdateShopMoney(int32 CurrentMoneyAmount)
{
	ShopWidget->UpdateMoneyAmount(CurrentMoneyAmount);
}

void AIRUIPlayerController::SetLanguage(FString InLanguage)
{
	FInternationalization::Get().SetCurrentCulture(InLanguage);

	AIRGameModeBase* GameModeBase = Cast<AIRGameModeBase>(GetWorld()->GetAuthGameMode());
	GameModeBase->SetLanguageData(InLanguage);
}

void AIRUIPlayerController::SetSensitivity(float InSensitivity)
{
	AIRGameModeBase* GameModeBase = Cast<AIRGameModeBase>(GetWorld()->GetAuthGameMode());
	GameModeBase->SetSensitivityData(InSensitivity);
}

void AIRUIPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeUIOnly InputMode;
	SetInputMode(InputMode);

	TitleWidget = CreateWidget<UIRTitleWidget>(this, TitleWidgetClass);
	ShopWidget = CreateWidget<UIRShopWidget>(this, ShopWidgetClass);

	if (TitleWidget)
	{
		TitleWidget->AddToViewport();
	}
	bShowMouseCursor = true;

	AudioComponent->SetSound(Cast<USoundBase>(BGMSoundCue));
	AudioComponent->Play();

	AIRGameModeBase* GameModeBase = Cast<AIRGameModeBase>(GetWorld()->GetAuthGameMode());
	UIRSaveGame* SaveGameInstance = GameModeBase->GetSaveGameInstance();
	FInternationalization::Get().SetCurrentCulture(SaveGameInstance->CurrentLanguage);

	SetVolumeToDefault();
	EquipItems();

	SaveGameInstance->UserName = GetPlayerState<APlayerState>()->GetPlayerName();
	GameModeBase->SaveGameData();
}

void AIRUIPlayerController::SetCameraTransform(bool IsEnterShop)
{
	AIRCharacterShopPlayer* ShopPlayer = Cast<AIRCharacterShopPlayer>(GetPawn());
	if (ShopPlayer)
	{
		ShopPlayer->SetCameraTransform(IsEnterShop);
	}
}

void AIRUIPlayerController::DisplaySettingWidget(EWidgetType InType)
{
	UIWidget = CreateWidget<UUserWidget>(this, SettingWidgetClass);
}

void AIRUIPlayerController::DisplayRankingWidget(EWidgetType InType)
{
	UIWidget = CreateWidget<UUserWidget>(this, RankingWidgetClass);
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
