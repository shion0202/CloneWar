// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/IRUserWidget.h"
#include "IRItemInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class THEINFINITYROOM_API UIRItemInfoWidget : public UIRUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	void UpdateItemInformation(FText Name, FText Description);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TXT_ItemName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TXT_ItemDescription;
};
