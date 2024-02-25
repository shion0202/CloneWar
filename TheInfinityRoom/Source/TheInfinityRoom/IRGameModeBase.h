// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "IRGameInterface.h"
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

	virtual void OnStageGoToNext(int32 NewStageLevel) override;
	virtual void OnPlayerDead() override;
	virtual int32 GetStageLevel() override;

protected:
	virtual void PostInitializeComponents() override;

protected:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = Game)
	int32 StageLevel;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SaveGame)
	TObjectPtr<class UIRSaveGame> SaveGameInstance;
};
