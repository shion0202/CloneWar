// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IRSettingWidget.generated.h"

/**
 * 
 */
UCLASS()
class THEINFINITYROOM_API UIRSettingWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnApplyClick();

	UFUNCTION()
	void OnCloseClick();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Volume)
	float BGMValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Volume)
	float SEValue;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BTN_Apply;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BTN_Close;
};
