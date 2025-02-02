// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Interface/IRGameInterface.h"
#include "IRGameModeBase.generated.h"

UCLASS()
class THEINFINITYROOM_API AIRGameModeBase : public AGameModeBase, public IIRGameInterface
{
	GENERATED_BODY()
	
public:
	AIRGameModeBase();

	void SaveGameData();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE class UIRSaveGame* GetSaveGameInstance() { return SaveGameInstance; }

	UFUNCTION(BlueprintCallable)
	void SetVolumeData(float InBGMValue, float InSEValue);

	void SetLanguageData(FString InLanguage);
	void SetSensitivityData(float InSensitivity);

	virtual void OnStageGoToNext(int32 NewStageLevel) override;
	virtual void OnDeliverEnemyCount(int32 DestroyEnemyCount, int32 TargetEnemyCount) override;
	virtual void OnChangeObjective(bool IsReady) override;
	virtual void OnReturnReward(int32 NewRewardAmount) override;
	virtual void OnPlayerDead() override;
	virtual int32 GetStageLevel() override;
	virtual int32 GetRewardAmount() override;

	virtual void ClearStage(int32 InClearedStage) override;
	virtual void KillEnemy(int32 InEnemyAmount) override;
	virtual void GetMoney(int32 InMoneyAmount) override;
	virtual void CountJump() override;
	virtual void UploadStageLevel() override;
	virtual void UploadHealAmount(int32 InHealAmount) override;
	virtual void UploadNewGameCount() override;
	virtual void ClearSplendorAchievements(int32 InClearedStage) override;

protected:
	virtual void PostInitializeComponents() override;

protected:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = Game)
	int32 StageLevel;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = Game)
	int32 RecentRewardAmount;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SaveGame)
	TObjectPtr<class UIRSaveGame> SaveGameInstance;
};
