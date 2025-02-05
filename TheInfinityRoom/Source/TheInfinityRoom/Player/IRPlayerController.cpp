// Fill out your copyright notice in the Description page of Project Settings.

#include "IRPlayerController.h"
#include "Async/Async.h"
#include "UI/IRHUDWidget.h"
#include "UI/IRGameOverWidget.h"
#include "UI/IRPauseWidget.h"
#include "Character/IRCharacterPlayer.h"

AIRPlayerController::AIRPlayerController()
{
	static ConstructorHelpers::FClassFinder<UIRHUDWidget> HUDWidgetRef(TEXT(
		"/Game/UI/WBP_HUD.WBP_HUD_C"));
	if (HUDWidgetRef.Class)
	{
		HUDWidgetClass = HUDWidgetRef.Class;
	}

	static ConstructorHelpers::FClassFinder<UIRGameOverWidget> GameOverWidgetRef(TEXT(
		"/Game/UI/WBP_GameOver.WBP_GameOver_C"));
	if (GameOverWidgetRef.Class)
	{
		GameOverWidgetClass = GameOverWidgetRef.Class;
	}

	static ConstructorHelpers::FClassFinder<UIRPauseWidget> PauseWidgetRef(TEXT(
		"/Game/UI/WBP_Pause.WBP_Pause_C"));
	if (PauseWidgetRef.Class)
	{
		PauseWidgetClass = PauseWidgetRef.Class;
	}
}

void AIRPlayerController::GameOver()
{
	GetWorld()->GetTimerManager().SetTimer(WaitingTimeHandle, this, &AIRPlayerController::DisplayGameOverWidget, 2.f, false);
}

void AIRPlayerController::OnGamePause()
{
	AIRCharacterPlayer* CharacterPlayer = Cast<AIRCharacterPlayer>(GetPawn());
	if (CharacterPlayer)
	{
		if (CharacterPlayer->GetCurrentControlType() == ECharacterControlType::Default)
		{
			FInputModeGameAndUI InputMode;
			SetInputMode(InputMode);

			PauseWidget = CreateWidget<UIRPauseWidget>(this, PauseWidgetClass);
			if (PauseWidget)
			{
				PauseWidget->AddToViewport();
			}

			IIRGameOverInterface* PauseInterface = Cast<IIRGameOverInterface>(GetPawn());
			if (PauseInterface)
			{
				PauseInterface->ChangeCharacterControl();
			}

			SetPause(true);
		}
		else if (CharacterPlayer->GetCurrentControlType() == ECharacterControlType::UI)
		{
			PauseWidget->OnResumeClicked();
		}
	}
}

void AIRPlayerController::ClearStage(int32 InClearedStage)
{
	if (SteamUserStats() == nullptr)
	{
		return;
	}

	bool bAchieved = false;
	if (InClearedStage == 1)
	{
		if (SteamUserStats()->GetAchievement("ACH_CLEAR_STAGE_1", &bAchieved) && !bAchieved)
		{
			SteamUserStats()->SetAchievement("ACH_CLEAR_STAGE_1");
		}
	}
	else if (InClearedStage == 10)
	{
		if (SteamUserStats()->GetAchievement("ACH_CLEAR_STAGE_10", &bAchieved) && !bAchieved)
		{
			SteamUserStats()->SetAchievement("ACH_CLEAR_STAGE_10");
		}
	}
	else if (InClearedStage == 20)
	{
		if (SteamUserStats()->GetAchievement("ACH_CLEAR_STAGE_20", &bAchieved) && !bAchieved)
		{
			SteamUserStats()->SetAchievement("ACH_CLEAR_STAGE_20");
		}
	}
	else
	{
		return;
	}

	if (!SteamUserStats()->StoreStats())
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to store stats on steam server."));
	}
}

void AIRPlayerController::KillEnemy(int32 InEnemyAmount)
{
	if (SteamUserStats() == nullptr)
	{
		return;
	}

	int32 CurrentStat = 0;
	if (SteamUserStats()->GetStat("KillEnemyAmount_KillEnemyAmount", &CurrentStat))
	{
		CurrentStat += InEnemyAmount;
		SteamUserStats()->SetStat("KillEnemyAmount_KillEnemyAmount", CurrentStat);
	}

	if (SteamUserStats()->StoreStats())
	{
		SteamAPICall_t hSteamAPICall = SteamUserStats()->FindLeaderboard("KillEnemyAmount");
		if (hSteamAPICall != k_uAPICallInvalid)
		{
			FindKillEnemyLeaderboardCallResult.Set(hSteamAPICall, this, &AIRPlayerController::OnFindLeaderboardKillEnemy);
		}
	}
}

void AIRPlayerController::GetMoney(int32 InMoneyAmount)
{
	if (SteamUserStats() == nullptr)
	{
		return;
	}

	int32 CurrentStat = 0;
	if (SteamUserStats()->GetStat("GetMoneyAmount_GetMoneyAmount", &CurrentStat))
	{
		CurrentStat += InMoneyAmount;
		SteamUserStats()->SetStat("GetMoneyAmount_GetMoneyAmount", CurrentStat);
	}

	if (SteamUserStats()->StoreStats())
	{
		SteamAPICall_t hSteamAPICall = SteamUserStats()->FindLeaderboard("GetMoneyAmount");
		if (hSteamAPICall != k_uAPICallInvalid)
		{
			FindGetMoneyLeaderboardCallResult.Set(hSteamAPICall, this, &AIRPlayerController::OnFindLeaderboardGetMoney);
		}
	}
}

void AIRPlayerController::CountJump()
{
	if (SteamUserStats() == nullptr)
	{
		return;
	}

	int32 CurrentStat = 0;
	if (SteamUserStats()->GetStat("NumberOfJumps_NumberOfJumps", &CurrentStat))
	{
		CurrentStat += 1;
		SteamUserStats()->SetStat("NumberOfJumps_NumberOfJumps", CurrentStat);
	}

	if (!SteamUserStats()->StoreStats())
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to store stats on steam server."));
	}
}

void AIRPlayerController::UploadStageLevel(int32 InStageLevel)
{
	if (SteamUserStats() == nullptr)
	{
		return;
	}

	int32 CurrentStat = 0;
	if (SteamUserStats()->GetStat("HighestStageLevel_HighestStageLevel", &CurrentStat))
	{
		if (InStageLevel <= CurrentStat)
		{
			return;
		}

		CurrentStat = InStageLevel;
		SteamUserStats()->SetStat("HighestStageLevel_HighestStageLevel", CurrentStat);
	}

	if (SteamUserStats()->StoreStats())
	{
		SteamAPICall_t hSteamAPICall = SteamUserStats()->FindLeaderboard("HighestStageLevel");
		if (hSteamAPICall != k_uAPICallInvalid)
		{
			FindStageLevelLeaderboardCallResult.Set(hSteamAPICall, this, &AIRPlayerController::OnFindLeaderboardStageLevel);
		}
	}
}

void AIRPlayerController::UploadHealAmount(int32 InHealAmount)
{
	if (SteamUserStats() == nullptr)
	{
		return;
	}

	int32 CurrentStat = 0;
	if (SteamUserStats()->GetStat("HealAmount_HealAmount", &CurrentStat))
	{
		CurrentStat += InHealAmount;
		SteamUserStats()->SetStat("HealAmount_HealAmount", CurrentStat);
	}

	if (!SteamUserStats()->StoreStats())
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to store stats on steam server."));
	}
}

void AIRPlayerController::UploadNewGameCount()
{
	if (SteamUserStats() == nullptr)
	{
		return;
	}

	int32 CurrentStat = 0;
	if (SteamUserStats()->GetStat("NewGameAmount_NewGameAmount", &CurrentStat))
	{
		CurrentStat += 1;
		SteamUserStats()->SetStat("NewGameAmount_NewGameAmount", CurrentStat);
	}

	if (!SteamUserStats()->StoreStats())
	{
		SteamAPICall_t hSteamAPICall = SteamUserStats()->FindLeaderboard("NewGameAmount");
		if (hSteamAPICall != k_uAPICallInvalid)
		{
			FindNewGameLeaderboardCallResult.Set(hSteamAPICall, this, &AIRPlayerController::OnFindLeaderboardNewGameCount);
		}
	}
}

void AIRPlayerController::ClearSplendorAchievements(int32 InClearedStage)
{
	AIRCharacterPlayer* CharacterPlayer = Cast<AIRCharacterPlayer>(GetPawn());
	if (InClearedStage != 15 || SteamUserStats() == nullptr || !CharacterPlayer)
	{
		return;
	}

	TArray<bool> bIsGettingItems = CharacterPlayer->GetIsGettingArray();
	int32 Count = 0;
	bool bAchieved = false;

	if (!bIsGettingItems[0])
	{
		if (SteamUserStats()->GetAchievement("ACH_NO_WEAPON_CLEAR", &bAchieved) && !bAchieved)
		{
			SteamUserStats()->SetAchievement("ACH_NO_WEAPON_CLEAR");
		}

		Count++;
	}
	if (!bIsGettingItems[1])
	{
		if (SteamUserStats()->GetAchievement("ACH_NO_BLUE_POTION_CLEAR", &bAchieved) && !bAchieved)
		{
			SteamUserStats()->SetAchievement("ACH_NO_BLUE_POTION_CLEAR");
		}

		Count++;
	}
	if (!bIsGettingItems[2])
	{
		if (SteamUserStats()->GetAchievement("ACH_NO_RED_POTION_CLEAR", &bAchieved) && !bAchieved)
		{
			SteamUserStats()->SetAchievement("ACH_NO_RED_POTION_CLEAR");
		}

		Count++;
	}
	if (Count == 3)
	{
		if (SteamUserStats()->GetAchievement("ACH_NO_ITEM_CLEAR", &bAchieved) && !bAchieved)
		{
			SteamUserStats()->SetAchievement("ACH_NO_ITEM_CLEAR");
		}
	}

	if (!SteamUserStats()->StoreStats())
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to store stats on steam server."));
	}
}

void AIRPlayerController::BeginPlay()
{
	Super::BeginPlay();

	HUDWidget = CreateWidget<UIRHUDWidget>(this, HUDWidgetClass);
	if (HUDWidget)
	{
		HUDWidget->AddToViewport();
	}

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
}

void AIRPlayerController::DisplayGameOverWidget()
{
	FInputModeUIOnly InputMode;
	SetInputMode(InputMode);

	GameOverWidget = CreateWidget<UIRGameOverWidget>(this, GameOverWidgetClass);
	if (GameOverWidget)
	{
		GameOverWidget->AddToViewport();
	}
}

void AIRPlayerController::OnFindLeaderboardKillEnemy(LeaderboardFindResult_t* pResult, bool bIOFailure)
{
	if (!bIOFailure && pResult->m_bLeaderboardFound)
	{
		SteamLeaderboard_t LeaderboardHandle = pResult->m_hSteamLeaderboard;

		int32 CurrentStat = 0;
		if (SteamUserStats()->GetStat("KillEnemyAmount_KillEnemyAmount", &CurrentStat))
		{
			SteamAPICall_t hSteamAPICall = SteamUserStats()->UploadLeaderboardScore(
				LeaderboardHandle, k_ELeaderboardUploadScoreMethodKeepBest, CurrentStat, nullptr, 0
			);

			if (hSteamAPICall != k_uAPICallInvalid)
			{
				UE_LOG(LogTemp, Warning, TEXT("Succeeded to find leaderboard."));
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to find leaderboard."));
	}
}

void AIRPlayerController::OnFindLeaderboardGetMoney(LeaderboardFindResult_t* pResult, bool bIOFailure)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1, 3.f, FColor::Red, FString::Printf(TEXT("Called."))
		);
	}

	if (!bIOFailure && pResult->m_bLeaderboardFound)
	{
		SteamLeaderboard_t LeaderboardHandle = pResult->m_hSteamLeaderboard;

		int32 CurrentStat = 0;
		if (SteamUserStats()->GetStat("GetMoneyAmount_GetMoneyAmount", &CurrentStat))
		{
			SteamAPICall_t hSteamAPICall = SteamUserStats()->UploadLeaderboardScore(
				LeaderboardHandle, k_ELeaderboardUploadScoreMethodKeepBest, CurrentStat, nullptr, 0
			);

			if (hSteamAPICall != k_uAPICallInvalid)
			{
				UE_LOG(LogTemp, Warning, TEXT("Succeeded to find leaderboard."));
			}

			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(
					-1, 3.f, FColor::Red, FString::Printf(TEXT("Get Money: %d"), CurrentStat)
				);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to find leaderboard."));
	}
}

void AIRPlayerController::OnFindLeaderboardStageLevel(LeaderboardFindResult_t* pResult, bool bIOFailure)
{
	SteamLeaderboard_t LeaderboardHandle = pResult->m_hSteamLeaderboard;

	if (!bIOFailure && pResult->m_bLeaderboardFound)
	{
		int32 CurrentStat = 0;
		if (SteamUserStats()->GetStat("HighestStageLevel_HighestStageLevel", &CurrentStat))
		{
			SteamAPICall_t hSteamAPICall = SteamUserStats()->UploadLeaderboardScore(
				LeaderboardHandle, k_ELeaderboardUploadScoreMethodKeepBest, CurrentStat, nullptr, 0
			);

			if (hSteamAPICall != k_uAPICallInvalid)
			{
				UE_LOG(LogTemp, Warning, TEXT("Succeeded to find leaderboard."));
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to find leaderboard."));
	}
}

void AIRPlayerController::OnFindLeaderboardNewGameCount(LeaderboardFindResult_t* pResult, bool bIOFailure)
{
	SteamLeaderboard_t LeaderboardHandle = pResult->m_hSteamLeaderboard;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Success Find!"));
	}

	if (!bIOFailure && pResult->m_bLeaderboardFound)
	{
		int32 CurrentStat = 0;
		if (SteamUserStats()->GetStat("NewGameAmount_NewGameAmount", &CurrentStat))
		{
			SteamAPICall_t hSteamAPICall = SteamUserStats()->UploadLeaderboardScore(
				LeaderboardHandle, k_ELeaderboardUploadScoreMethodKeepBest, CurrentStat, nullptr, 0
			);

			if (hSteamAPICall != k_uAPICallInvalid)
			{
				UE_LOG(LogTemp, Warning, TEXT("Succeeded to find leaderboard."));
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to find leaderboard."));
	}
}

void AIRPlayerController::OnGameOverlayActivated(GameOverlayActivated_t* pCallback)
{
	// Activate steam overlay.
	AIRCharacterPlayer* CharacterPlayer = Cast<AIRCharacterPlayer>(GetPawn());
	if (pCallback->m_bActive && CharacterPlayer)
	{
		if (CharacterPlayer->GetCurrentControlType() == ECharacterControlType::Default)
		{
			AsyncTask(ENamedThreads::GameThread, [this]() {
				OnGamePause();
			});
		}
	}
}
