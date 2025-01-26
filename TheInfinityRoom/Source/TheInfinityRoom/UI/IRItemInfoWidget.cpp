// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/IRItemInfoWidget.h"
#include "Components/TextBlock.h"
#include "Interface/IRItemWidgetInterface.h"

void UIRItemInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	IIRItemWidgetInterface* ItemInfoWidget = Cast<IIRItemWidgetInterface>(Owner);
	if (ItemInfoWidget)
	{
		ItemInfoWidget->SetupSpaceWidget(this);
	}
}

void UIRItemInfoWidget::UpdateItemInformation(FText Name, FText Description)
{
	TXT_ItemName->SetText(Name);
	TXT_ItemDescription->SetText(Description);
}
