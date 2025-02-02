// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IRGameInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIRGameInterface : public UInterface
{
	GENERATED_BODY()
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnStageLevelChangedDelegate, int32 /* StageLevel */);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnEnemyCountChangedDelegate, int32 /* DestroyEnemyCount */, int32 /* TargetEnemyCount */);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnObjectiveChangedDelegate, bool /* IsReady */);
DECLARE_MULTICAST_DELEGATE(FOnGameOverDelegate);

/**
 * 
 */
class THEINFINITYROOM_API IIRGameInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void OnStageGoToNext(int32 NewStageLevel) = 0;
	virtual void OnDeliverEnemyCount(int32 DestroyEnemyCount, int32 TargetEnemyCount) = 0;
	virtual void OnChangeObjective(bool IsReady) = 0;
	virtual void OnReturnReward(int32 NewRewardAmount) = 0;
	virtual void OnPlayerDead() = 0;
	virtual int32 GetStageLevel() = 0;
	virtual int32 GetRewardAmount() = 0;

	virtual void ClearStage(int32 InClearedStage) = 0;
	virtual void KillEnemy(int32 InEnemyAmount) = 0;
	virtual void GetMoney(int32 InMoneyAmount) = 0;
	virtual void CountJump() = 0;
	virtual void UploadStageLevel() = 0;
	virtual void UploadHealAmount(int32 InHealAmount) = 0;
	virtual void UploadNewGameCount() = 0;
	virtual void ClearSplendorAchievements(int32 InClearedStage) = 0;

	FOnStageLevelChangedDelegate OnStageLevelChanged;
	FOnEnemyCountChangedDelegate OnEnemyCountChanged;
	FOnObjectiveChangedDelegate OnObjectiveChanged;
	FOnGameOverDelegate OnGameOver;
};
