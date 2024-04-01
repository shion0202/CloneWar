// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
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
	UFUNCTION(BlueprintImplementableEvent, Category = SteamFunc)
	void ClearStage1();

	UFUNCTION(BlueprintImplementableEvent, Category = SteamFunc)
	void ClearStage10();

	UFUNCTION(BlueprintImplementableEvent, Category = SteamFunc)
	void KillEnemy();

protected:
	virtual void BeginPlay();
	
	void DisplayGameOverWidget();

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
