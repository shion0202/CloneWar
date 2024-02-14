// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IRPauseWidget.generated.h"

/**
 * 
 */
UCLASS()
class THEINFINITYROOM_API UIRPauseWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void OnResumeClicked();
	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnRetryClicked();

	UFUNCTION()
	void OnReturnToTitleClicked();

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BTN_Resume;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BTN_Retry;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BTN_ReturnToTitle;
};
