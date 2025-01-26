// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IRStageLevelWidget.generated.h"

/**
 * 
 */
UCLASS()
class THEINFINITYROOM_API UIRStageLevelWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	void UpdateStageLevel(int32 CurrentStageLevel);
	void UpdateNumOfEnemies(int32 InDestroyEnemyCount, int32 InTargetEnemyCount);
	void UpdateObjective(bool IsReady);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TXT_StageLevel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TXT_NumOfEnemies;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TXT_Objective;
};
