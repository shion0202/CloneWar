// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "steam_api.h"
#include "IRGameInstance.generated.h"

UCLASS()
class THEINFINITYROOM_API UIRGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UIRGameInstance();

    virtual void Init();
	virtual void Shutdown();

	void ProcessSteamCallbacks();

	void DownloadScores();
	void DownloadLeaderboard();

	STEAM_CALLBACK(UIRGameInstance, OnUserStatsReceived, UserStatsReceived_t);
	// void OnUserStatsReceived(UserStatsReceived_t* pCallback);

	TMap<FString, int32> GetStatScore();
	
private:
	void OnFindLeaderboard(LeaderboardFindResult_t* pResult, bool bIOFailure);
	void OnScoresDownloaded(LeaderboardScoresDownloaded_t* pResult, bool bIOFailure);

private:
	SteamLeaderboard_t LeaderboardHandle;
	LeaderboardEntry_t Entry;

	CCallResult<UIRGameInstance, LeaderboardFindResult_t> CallbackFind;
	CCallResult<UIRGameInstance, LeaderboardScoresDownloaded_t> CallbackDownload;

	TArray<FString> StatNames = {
		"NewGameAmount_NewGameAmount", "HighestStageLevel_HighestStageLevel", "KillEnemyAmount_KillEnemyAmount", "GetMoneyAmount_GetMoneyAmount", "UseMoneyAmount_UseMoneyAmount"
	};
	TMap<FString, int32> StatScores;
};
