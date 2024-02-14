// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IRGameOverWidget.generated.h"

/**
 * 
 */
UCLASS()
class THEINFINITYROOM_API UIRGameOverWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnRetryClicked();

	UFUNCTION()
	void OnReturnToTitleClicked();

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TXT_StageLevel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TXT_Reward;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BTN_Retry;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BTN_ReturnToTitle;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<class UWidgetAnimation> Fade;
};
