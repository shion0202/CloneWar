// Fill out your copyright notice in the Description page of Project Settings.

#include "IRUIPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "IRTitleWidget.h"

AIRUIPlayerController::AIRUIPlayerController()
{
	static ConstructorHelpers::FClassFinder<UIRTitleWidget> TitleWidgetRef(TEXT(
		"/Game/UI/WBP_Title.WBP_Title_C"));
	if (TitleWidgetRef.Class)
	{
		TitleWidgetClass = TitleWidgetRef.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> HelpWidgetRef(TEXT(
		"/Game/UI/WBP_Help.WBP_Help_C"));
	if (HelpWidgetRef.Class)
	{
		HelpWidgetClass = HelpWidgetRef.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> LicenseWidgetRef(TEXT(
		"/Game/UI/WBP_License.WBP_License_C"));
	if (LicenseWidgetRef.Class)
	{
		LicenseWidgetClass = LicenseWidgetRef.Class;
	}
}

void AIRUIPlayerController::OnDisplayWidget(EWidgetType Type)
{
	if (Type == EWidgetType::Help)
	{
		UIWidget = CreateWidget<UUserWidget>(this, HelpWidgetClass);
	}
	else if (Type == EWidgetType::License)
	{
		UIWidget = CreateWidget<UUserWidget>(this, LicenseWidgetClass);
	}

	if (UIWidget)
	{
		UIWidget->AddToViewport();
	}
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
}
