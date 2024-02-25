// Fill out your copyright notice in the Description page of Project Settings.

#include "IRListItemWidget.h"
#include "Components/TextBlock.h"
#include "IRCreditObject.h"

void UIRListItemWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	UIRCreditObject* Item = Cast<UIRCreditObject>(ListItemObject);
	if (Item != nullptr)
	{
		TXT_Name->SetText(Item->GetCreditData().Name);
		TXT_Author->SetText(Item->GetCreditData().Author);
		TXT_Link->SetText(Item->GetCreditData().Link);
	}
}
