// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IRUserWidget.h"
#include "IRRuleWidget.generated.h"

/**
 * 
 */
UCLASS()
class THEINFINITYROOM_API UIRRuleWidget : public UIRUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnCloseClick();

	UFUNCTION()
	void OnLeftClick();

	UFUNCTION()
	void OnRightClick();

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BTN_Close;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BTN_Left;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BTN_Right;
};
