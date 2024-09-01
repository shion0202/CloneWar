// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IRTutorialWidget.generated.h"

/**
 * 
 */
UCLASS()
class THEINFINITYROOM_API UIRTutorialWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnCloseClick();

	UFUNCTION()
	void OnLeftClick();

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BTN_Close;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BTN_Left;
};
