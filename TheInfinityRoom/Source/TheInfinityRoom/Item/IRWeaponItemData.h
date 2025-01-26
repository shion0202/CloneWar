// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IRItemData.h"
#include "GameData/IRCharacterStat.h"
#include "IRWeaponItemData.generated.h"

UCLASS()
class THEINFINITYROOM_API UIRWeaponItemData : public UIRItemData
{
	GENERATED_BODY()
	
protected:
	UIRWeaponItemData();
	
public:
	FPrimaryAssetId GetPrimaryAssetId() const override;

	FORCEINLINE FIRCharacterStat GetWeaponStat() { return WeaponStat; }
	
protected:
	UPROPERTY(EditAnywhere, Category = Stat)
	FIRCharacterStat WeaponStat;
};
