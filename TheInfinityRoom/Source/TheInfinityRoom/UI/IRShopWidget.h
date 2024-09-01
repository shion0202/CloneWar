// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IRShopWidget.generated.h"

/**
 * 
 */
UCLASS()
class THEINFINITYROOM_API UIRShopWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UIRShopWidget(const FObjectInitializer& ObjectInitializer);

	void UpdateMoneyAmount(int32 CurrentMoneyAmount);

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnReturnClicked();

protected:
	UPROPERTY()
	TObjectPtr<class UIRMoneyWidget> Money;

	UPROPERTY()
	TObjectPtr<class UIRShopListWidget> ShopList;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> BTN_ReturnToTitle;
};
