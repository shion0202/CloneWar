// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "steam_api.h"
#include "IRPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class THEINFINITYROOM_API AIRPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AIRPlayerController();

	void GameOver();
	void OnGamePause();

public:
	UFUNCTION(Category = Steam)
	void ClearStage(int32 InClearedStage);

	UFUNCTION(Category = Steam)
	void KillEnemy(int32 InEnemyAmount);

	UFUNCTION(Category = Steam)
	void GetMoney(int32 InMoneyAmount);

	UFUNCTION(Category = Steam)
	void CountJump();

	UFUNCTION(Category = Steam)
	void UploadStageLevel(int32 InStageLevel);

	UFUNCTION(Category = Steam)
	void UploadHealAmount(int32 InHealAmount);

	UFUNCTION(Category = Steam)
	void UploadNewGameCount();

protected:
	virtual void BeginPlay();
	
	void DisplayGameOverWidget();

	void OnFindLeaderboardKillEnemy(LeaderboardFindResult_t* pResult, bool bIOFailure);
	void OnFindLeaderboardGetMoney(LeaderboardFindResult_t* pResult, bool bIOFailure);
	void OnFindLeaderboardStageLevel(LeaderboardFindResult_t* pResult, bool bIOFailure);
	void OnFindLeaderboardNewGameCount(LeaderboardFindResult_t* pResult, bool bIOFailure);

private:
	CCallResult<AIRPlayerController, LeaderboardFindResult_t> FindLeaderboardCallResult;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class UIRHUDWidget> HUDWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUD)
	TObjectPtr<class UIRHUDWidget> HUDWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class UIRGameOverWidget> GameOverWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUD)
	TObjectPtr<class UIRGameOverWidget> GameOverWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class UIRPauseWidget> PauseWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUD)
	TObjectPtr<class UIRPauseWidget> PauseWidget;

	FTimerHandle WaitingTimeHandle;
};
