// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "IRCredit.h"
#include "IRListItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class THEINFINITYROOM_API UIRListItemWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TXT_Name;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TXT_Author;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TXT_Link;
};
