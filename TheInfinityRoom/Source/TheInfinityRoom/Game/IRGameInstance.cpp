// Fill out your copyright notice in the Description page of Project Settings.

#include "IRGameInstance.h"

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

		FTimerHandle SteamCallbackTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(SteamCallbackTimerHandle, this, &UIRGameInstance::ProcessSteamCallbacks, 0.1f, true);
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

void UIRGameInstance::DownloadLeaderboard()
{
	SteamAPICall_t hSteamAPICall = SteamUserStats()->FindLeaderboard("HighestStageLevel");
	CallbackFind.Set(hSteamAPICall, this, &UIRGameInstance::OnFindLeaderboard);
}

TMap<FString, int32> UIRGameInstance::GetStatScore()
{
	return StatScores;
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

void UIRGameInstance::OnFindLeaderboard(LeaderboardFindResult_t* pResult, bool bIOFailure)
{
	//if (pResult->m_bLeaderboardFound && !bIOFailure)
	//{
	//	LeaderboardHandle = pResult->m_hSteamLeaderboard;

	//	SteamAPICall_t hSteamAPICall = SteamUserStats()->DownloadLeaderboardEntries(
	//		LeaderboardHandle, k_ELeaderboardDataRequestGlobal, 1, 10
	//	);
	//	CallbackDownload.Set(hSteamAPICall, this, &UIRRankingWidget::OnScoresDownloaded);
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Failed to find leaderboard."));
	//}

	if (pResult->m_bLeaderboardFound && !bIOFailure)
	{
		LeaderboardHandle = pResult->m_hSteamLeaderboard;

		SteamAPICall_t hSteamAPICall = SteamUserStats()->DownloadLeaderboardEntries(
			LeaderboardHandle, k_ELeaderboardDataRequestGlobalAroundUser, -1, 1
		);
		CallbackDownload.Set(hSteamAPICall, this, &UIRGameInstance::OnScoresDownloaded);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to find leaderboard."));
	}
}

void UIRGameInstance::OnScoresDownloaded(LeaderboardScoresDownloaded_t* pResult, bool bIOFailure)
{
	//if (!bIOFailure && pResult->m_cEntryCount > 0)
	//{
	//	for (int i = 0; i < pResult->m_cEntryCount; ++i)
	//	{
	//		LeaderboardEntry_t Entry;
	//		SteamUserStats()->GetDownloadedLeaderboardEntry(pResult->m_hSteamLeaderboardEntries, i, &Entry, nullptr, 0);

	//		if (GEngine)
	//		{
	//			GEngine->AddOnScreenDebugMessage(
	//				-1, 15.f, FColor::Orange, FString::Printf(TEXT("%llu (No.%d)"), Entry.m_steamIDUser.ConvertToUint64(), Entry.m_nGlobalRank)
	//			);
	//		}
	//	}
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Faild to download scores."));
	//}

	if (!bIOFailure && pResult->m_cEntryCount > 0)
	{
		SteamUserStats()->GetDownloadedLeaderboardEntry(pResult->m_hSteamLeaderboardEntries, 0, &Entry, nullptr, 0);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Faild to download scores."));
	}
}
