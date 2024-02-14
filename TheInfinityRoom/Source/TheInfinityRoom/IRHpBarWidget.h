// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IRUserWidget.h"
#include "IRHpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class THEINFINITYROOM_API UIRHpBarWidget : public UIRUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	
public:
	void UpdateHp(float InMaxHp, float InCurrentHp);
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> PB_HpBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TXT_HpRatio;
};
