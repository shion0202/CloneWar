// Fill out your copyright notice in the Description page of Project Settings.

#include "IRGameInstance.h"
#include "Containers/Ticker.h"

UIRGameInstance::UIRGameInstance()
{

}

void UIRGameInstance::Init()
{
    Super::Init();

    if (SteamAPI_RestartAppIfNecessary(2918600))
    {
        FGenericPlatformMisc::RequestExit(false);
    }

    if (SteamAPI_Init())
    {
		DownloadScores();
		DownloadLeaderboards();

		FTicker::GetCoreTicker().AddTicker(
			FTickerDelegate::CreateUObject(this, &UIRGameInstance::OnTick),
			0.1f
		);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to initialize Steam API."));
    }
}

void UIRGameInstance::Shutdown()
{
    Super::Shutdown();
    SteamAPI_Shutdown();
}

void UIRGameInstance::ProcessSteamCallbacks()
{
	SteamAPI_RunCallbacks();
}

void UIRGameInstance::DownloadScores()
{
	bool Success = SteamUserStats()->RequestCurrentStats();
	if (!Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to request current stats."));
	}
}

void UIRGameInstance::DownloadLeaderboards()
{
	SteamAPICall_t hSteamAPICallStageLevel = SteamUserStats()->FindLeaderboard("HighestStageLevel");
	FindStageLevelLeaderboardCallResult.Set(hSteamAPICallStageLevel, this, &UIRGameInstance::OnFindStageLevelLeaderboard);

	SteamAPICall_t hSteamAPICallKillEnemies = SteamUserStats()->FindLeaderboard("KillEnemyAmount");
	FindKillEnemyLeaderboardCallResult.Set(hSteamAPICallKillEnemies, this, &UIRGameInstance::OnFindKillEnemyLeaderboard);
}

bool UIRGameInstance::OnTick(float DeltaTime)
{
	ProcessSteamCallbacks();
	return true;
}

void UIRGameInstance::OnFindStageLevelLeaderboard(LeaderboardFindResult_t* pResult, bool bIOFailure)
{
	if (pResult->m_bLeaderboardFound && !bIOFailure) {
		SteamLeaderboard_t Leaderboard = pResult->m_hSteamLeaderboard;
		SteamAPICall_t hSteamAPICall =
			SteamUserStats()->DownloadLeaderboardEntries(Leaderboard, k_ELeaderboardDataRequestGlobal, 1, 10);
		DownloadStageLevelEntriesCallResult.Set(hSteamAPICall, this, &UIRGameInstance::OnDownloadStageLevelEntries);

		CSteamID MySteamID = SteamUser()->GetSteamID();
		SteamAPICall_t hMySteamAPICall = SteamUserStats()->DownloadLeaderboardEntriesForUsers(Leaderboard, &MySteamID, 1);
		DownloadMyStageLevelEntriesCallResult.Set(hMySteamAPICall, this, &UIRGameInstance::OnDownloadMyStageLevelEntries);
	}
}

void UIRGameInstance::OnFindKillEnemyLeaderboard(LeaderboardFindResult_t* pResult, bool bIOFailure)
{
	if (pResult->m_bLeaderboardFound && !bIOFailure) {
		SteamLeaderboard_t Leaderboard = pResult->m_hSteamLeaderboard;
		SteamAPICall_t hSteamAPICall =
			SteamUserStats()->DownloadLeaderboardEntries(Leaderboard, k_ELeaderboardDataRequestGlobal, 1, 10);
		DownloadKillEnemyEntriesCallResult.Set(hSteamAPICall, this, &UIRGameInstance::OnDownloadKillEnemyEntries);

		CSteamID MySteamID = SteamUser()->GetSteamID();
		SteamAPICall_t hMySteamAPICall = SteamUserStats()->DownloadLeaderboardEntriesForUsers(Leaderboard, &MySteamID, 1);
		DownloadMyKillEnemyEntriesCallResult.Set(hMySteamAPICall, this, &UIRGameInstance::OnDownloadMyKillEnemyEntries);
	}
}

void UIRGameInstance::OnDownloadStageLevelEntries(LeaderboardScoresDownloaded_t* pResult, bool bIOFailure)
{
	for (int i = 0; i < pResult->m_cEntryCount; ++i) {
		LeaderboardEntry_t Entry;
		SteamUserStats()->GetDownloadedLeaderboardEntry(pResult->m_hSteamLeaderboardEntries, i, &Entry, nullptr, 0);

		FString DecodedName = FString(UTF8_TO_TCHAR(SteamFriends()->GetFriendPersonaName(Entry.m_steamIDUser)));

		FLeaderboardRow RowData;
		RowData.Rank = Entry.m_nGlobalRank;
		RowData.Name = DecodedName;
		RowData.Score = Entry.m_nScore;
		StageLevelLeaderboards.Add(RowData.Rank, RowData);
	}
}

void UIRGameInstance::OnDownloadKillEnemyEntries(LeaderboardScoresDownloaded_t* pResult, bool bIOFailure)
{
	for (int i = 0; i < pResult->m_cEntryCount; ++i) {
		LeaderboardEntry_t Entry;
		SteamUserStats()->GetDownloadedLeaderboardEntry(pResult->m_hSteamLeaderboardEntries, i, &Entry, nullptr, 0);

		FString DecodedName = FString(UTF8_TO_TCHAR(SteamFriends()->GetFriendPersonaName(Entry.m_steamIDUser)));

		FLeaderboardRow RowData;
		RowData.Rank = Entry.m_nGlobalRank;
		RowData.Name = DecodedName;
		RowData.Score = Entry.m_nScore;
		KillEnemyLeaderboards.Add(RowData.Rank, RowData);
	}
}

void UIRGameInstance::OnDownloadMyStageLevelEntries(LeaderboardScoresDownloaded_t* pResult, bool bIOFailure)
{
	LeaderboardEntry_t Entry;
	SteamUserStats()->GetDownloadedLeaderboardEntry(pResult->m_hSteamLeaderboardEntries, 0, &Entry, nullptr, 0);

	FString DecodedName = FString(UTF8_TO_TCHAR(SteamFriends()->GetFriendPersonaName(SteamUser()->GetSteamID())));
	FLeaderboardRow RowData;
	RowData.Name = DecodedName;
	
	if (Entry.m_steamIDUser.IsValid())
	{
		RowData.Rank = Entry.m_nGlobalRank;
		RowData.Score = Entry.m_nScore;
	}
	else
	{
		RowData.Rank = 0;
		RowData.Score = 0;
	}

	MyLeaderboards.Add(0, RowData);
}

void UIRGameInstance::OnDownloadMyKillEnemyEntries(LeaderboardScoresDownloaded_t* pResult, bool bIOFailure)
{
	LeaderboardEntry_t Entry;
	SteamUserStats()->GetDownloadedLeaderboardEntry(pResult->m_hSteamLeaderboardEntries, 0, &Entry, nullptr, 0);

	FString DecodedName = FString(UTF8_TO_TCHAR(SteamFriends()->GetFriendPersonaName(SteamUser()->GetSteamID())));
	FLeaderboardRow RowData;
	RowData.Name = DecodedName;

	if (Entry.m_steamIDUser.IsValid())
	{
		RowData.Rank = Entry.m_nGlobalRank;
		RowData.Score = Entry.m_nScore;
	}
	else
	{
		RowData.Rank = 0;
		RowData.Score = 0;
	}

	MyLeaderboards.Add(1, RowData);
}

TMap<FString, int32> UIRGameInstance::GetStatScore()
{
	return StatScores;
}

TMap<int32, FLeaderboardRow> UIRGameInstance::GetLeaderboards(bool bIsStageLevel)
{
	if (bIsStageLevel)
	{
		return StageLevelLeaderboards;
	}
	else
	{
		return KillEnemyLeaderboards;
	}
}

TMap<int32, FLeaderboardRow> UIRGameInstance::GetMyLeaderboards()
{
	return MyLeaderboards;
}

void UIRGameInstance::OnUserStatsReceived(UserStatsReceived_t* pCallback)
{
	if (pCallback->m_nGameID == SteamUtils()->GetAppID())
	{
		if (pCallback->m_eResult == k_EResultOK)
		{
			int32 Score;
			bool bExistStats = false;
			for (auto StatName : StatNames)
			{
				Score = 0;
				if (SteamUserStats()->GetStat(StringCast<ANSICHAR>(*StatName).Get(), &Score))
				{
					StatScores.Add(StatName, Score);
					bExistStats = true;
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to receive stats."));
		}
	}
}
