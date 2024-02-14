// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IRTitleWidget.generated.h"

/**
 * 
 */
UCLASS()
class THEINFINITYROOM_API UIRTitleWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnSinglePlayClicked();

	UFUNCTION()
	void OnMultiPlayClicked();

	UFUNCTION()
	void OnShopClicked();

	UFUNCTION()
	void OnSettingClicked();

	UFUNCTION()
	void OnExitClicked();

	UFUNCTION()
	void OnHelpClicked();

	UFUNCTION()
	void OnLicenseClicked();

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> BTN_SinglePlay;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> BTN_MultiPlay;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> BTN_Shop;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> BTN_Setting;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> BTN_Exit;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> BTN_Help;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> BTN_License;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<class UWidgetAnimation> Slide;
};
