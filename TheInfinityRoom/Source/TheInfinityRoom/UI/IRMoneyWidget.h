// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IRMoneyWidget.generated.h"

/**
 * 
 */
UCLASS()
class THEINFINITYROOM_API UIRMoneyWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	void UpdateMoney(int32 CurrentMoneyAmount);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TXT_MoneyAmount;
};
