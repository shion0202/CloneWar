// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IRCharacterStat.h"
#include "IRHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class THEINFINITYROOM_API UIRHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UIRHUDWidget(const FObjectInitializer& ObjectInitializer);

public:
	void UpdateStat(const FIRCharacterStat& TotalStat);
	void UpdateStageLevel(int32 CurrentStageLevel);

protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY()
	TObjectPtr<class UIRStageLevelWidget> StageLevel;

	UPROPERTY()
	TObjectPtr<class UIRCharacterStatWidget> CharacterStat;
};
