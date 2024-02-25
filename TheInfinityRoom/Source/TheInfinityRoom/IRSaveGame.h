// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "IRSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class THEINFINITYROOM_API UIRSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UIRSaveGame();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Game)
	float BGMVolumeValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Game)
	float SEVolumeValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Game)
	FString CurrentLanguage;
};
