// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IRHelpWidget.generated.h"

/**
 * 
 */
UCLASS()
class THEINFINITYROOM_API UIRHelpWidget : public UUserWidget
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
