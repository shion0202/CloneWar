// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "steam_api.h"
#include "IRGameInstance.generated.h"

USTRUCT()
struct FLeaderboardRow
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	int32 Rank;

	UPROPERTY(EditAnywhere)
	FString Name;

	UPROPERTY(EditAnywhere)
	int32 Score;
};

UCLASS()
class THEINFINITYROOM_API UIRGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UIRGameInstance();

    virtual void Init();
	virtual void Shutdown();
	void ProcessSteamCallbacks();
	bool OnTick(float DeltaTime);

	void DownloadScores();
	void DownloadLeaderboards();

	STEAM_CALLBACK(UIRGameInstance, OnUserStatsReceived, UserStatsReceived_t);
	// void OnUserStatsReceived(UserStatsReceived_t* pCallback);

	void OnFindStageLevelLeaderboard(LeaderboardFindResult_t* pResult, bool bIOFailure);
	void OnFindKillEnemyLeaderboard(LeaderboardFindResult_t* pResult, bool bIOFailure);
	void OnDownloadStageLevelEntries(LeaderboardScoresDownloaded_t* pResult, bool bIOFailure);
	void OnDownloadKillEnemyEntries(LeaderboardScoresDownloaded_t* pResult, bool bIOFailure);
	void OnDownloadMyStageLevelEntries(LeaderboardScoresDownloaded_t* pResult, bool bIOFailure);
	void OnDownloadMyKillEnemyEntries(LeaderboardScoresDownloaded_t* pResult, bool bIOFailure);

	TMap<FString, int32> GetStatScore();
	TMap<int32, FLeaderboardRow> GetLeaderboards(bool bIsStageLevel);
	TMap<int32, FLeaderboardRow> GetMyLeaderboards();

private:
	TArray<FString> StatNames = {
		"NewGameAmount_NewGameAmount", "HighestStageLevel_HighestStageLevel", "KillEnemyAmount_KillEnemyAmount", "GetMoneyAmount_GetMoneyAmount", "UseMoneyAmount_UseMoneyAmount"
	};
	TMap<FString, int32> StatScores;

	CCallResult<UIRGameInstance, LeaderboardFindResult_t> FindStageLevelLeaderboardCallResult;
	CCallResult<UIRGameInstance, LeaderboardFindResult_t> FindKillEnemyLeaderboardCallResult;
	CCallResult<UIRGameInstance, LeaderboardScoresDownloaded_t> DownloadStageLevelEntriesCallResult;
	CCallResult<UIRGameInstance, LeaderboardScoresDownloaded_t> DownloadKillEnemyEntriesCallResult;
	CCallResult<UIRGameInstance, LeaderboardScoresDownloaded_t> DownloadMyStageLevelEntriesCallResult;
	CCallResult<UIRGameInstance, LeaderboardScoresDownloaded_t> DownloadMyKillEnemyEntriesCallResult;

	TMap<int32, FLeaderboardRow> StageLevelLeaderboards;
	TMap<int32, FLeaderboardRow> KillEnemyLeaderboards;
	TMap<int32, FLeaderboardRow> MyLeaderboards;
};
