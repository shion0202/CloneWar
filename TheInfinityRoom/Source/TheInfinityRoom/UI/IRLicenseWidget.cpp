// Fill out your copyright notice in the Description page of Project Settings.

#include "IRLicenseWidget.h"
#include "Components/Button.h"
#include "Components/ListView.h"
#include "Components/ScrollBox.h"
#include "GameData/IRCredit.h"
#include "GameData/IRGameSingleton.h"
#include "IRCreditObject.h"
#include "IRListItemWidget.h"
#include "Player/IRUIPlayerController.h"

void UIRLicenseWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (nullptr != BTN_Close)
	{
		BTN_Close->OnClicked.AddDynamic(this, &UIRLicenseWidget::OnCloseClick);
	}

	TArray<FIRCredit> CreditTable = UIRGameSingleton::Get().GetCredit();
	for (FIRCredit Credit : CreditTable)
	{
		UIRCreditObject* CreditObject = NewObject<UIRCreditObject>();
		CreditObject->SetCreditData(Credit);
		LV_Credit->AddItem(CreditObject);
	}
}

void UIRLicenseWidget::OnCloseClick()
{
	AIRUIPlayerController* PlayerController = Cast<AIRUIPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		PlayerController->EnableButtons();
	}

	RemoveFromParent();
}
