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
	
public:
	void SetIsPlayAnim(bool IsPlay);
	void DisableButtons();

	UFUNCTION(BlueprintCallable)
	void OnTitleAnimComplete();
	
protected:
	UIRTitleWidget(const FObjectInitializer& ObjectInitializer);

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
	void OnRankingClicked();

	UFUNCTION()
	void OnHelpClicked();

	UFUNCTION()
	void OnLicenseClicked();

	void SinglePlay();
	void Exit();

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> BTN_SinglePlay;

	//UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	//TObjectPtr<class UButton> BTN_MultiPlay;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> BTN_Shop;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> BTN_Setting;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> BTN_Exit;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> BTN_Ranking;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> BTN_Help;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> BTN_License;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<class UWidgetAnimation> Slide;

	UPROPERTY()
	TObjectPtr<class USoundWave> ClickSoundWave;

	uint8 bIsPlayAnim : 1;
};
