// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameData/IRCharacterStat.h"
#include "IRCharacterStatWidget.generated.h"

/**
 * 
 */
UCLASS()
class THEINFINITYROOM_API UIRCharacterStatWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	void UpdateStat(const FIRCharacterStat& TotalStat);

private:
	UPROPERTY()
	TMap<FName, class UProgressBar*> ProgressBarLookup;
	
	UPROPERTY()
	TMap<FName, float> MinLevelStatValues;

	UPROPERTY()
	TMap<FName, float> MaxLevelStatValues;
};
