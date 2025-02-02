// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IRRankingWidget.generated.h"

UCLASS()
class THEINFINITYROOM_API UIRRankingWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnRankStageClick();

	UFUNCTION()
	void OnRankKillClick();

	UFUNCTION()
	void OnCloseClick();

public:
	UFUNCTION()
	int32 GetStat(FString Key);
	
	UFUNCTION()
	void SetStats();

private:
	TMap<FString, int32> Stats;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TXT_NewGameCount;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TXT_HighestStageLevel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TXT_DestroyEnemyCount;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TXT_GetMoneyCount;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TXT_UseMoneyCount;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BTN_RankStage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BTN_RankKill;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BTN_Close;
};
