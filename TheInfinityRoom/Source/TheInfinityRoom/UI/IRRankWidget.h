// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Game/IRGameInstance.h"
#include "IRRankWidget.generated.h"

/**
 * 
 */
UCLASS()
class THEINFINITYROOM_API UIRRankWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnStatisticsClick();

	UFUNCTION()
	void OnRankKillClick();

	UFUNCTION()
	void OnCloseClick();

	void SetLeaderboards();

private:
	TArray<FLeaderboardRow> LeaderboardsList;
	FLeaderboardRow MyLeaderboard;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TXT_MyRank;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TXT_MyName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TXT_MyValue;

	UPROPERTY()
	TArray<class UTextBlock*> RankTextBlockLookup;

	UPROPERTY()
	TArray<class UTextBlock*> NameTextBlockLookup;

	UPROPERTY()
	TArray<class UTextBlock*> ValueTextBlockLookup;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BTN_Statistics;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BTN_RankKill;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BTN_Close;
};
