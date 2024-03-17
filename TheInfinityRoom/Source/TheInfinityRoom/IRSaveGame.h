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
	FString UserName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Game)
	float BGMVolumeValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Game)
	float SEVolumeValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Game)
	FString CurrentLanguage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Game)
	int32 MoneyAmount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Game)
	TMap<FName, bool> Inventory;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Game)
	TObjectPtr<USkeletalMesh> EquipedSkin;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Game)
	TObjectPtr<class UNiagaraSystem> EquipedEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Game)
	TMap<FName, TObjectPtr<UStaticMesh>> EquipedItems;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Game)
	FName HeadSocketName;
};
