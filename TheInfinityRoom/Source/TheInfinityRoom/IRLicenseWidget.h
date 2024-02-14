// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IRLicenseWidget.generated.h"

/**
 * 
 */
UCLASS()
class THEINFINITYROOM_API UIRLicenseWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnCloseClick();

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BTN_Close;
};
