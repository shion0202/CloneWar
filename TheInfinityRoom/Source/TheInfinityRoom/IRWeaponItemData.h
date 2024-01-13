// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IRItemData.h"
#include "IRCharacterStat.h"
#include "IRWeaponItemData.generated.h"

UCLASS()
class THEINFINITYROOM_API UIRWeaponItemData : public UIRItemData
{
	GENERATED_BODY()
	
protected:
	UIRWeaponItemData();
	
public:
	FORCEINLINE FIRCharacterStat GetWeaponStat() { return WeaponStat; }

	virtual class UStaticMesh* GetItemMesh() override;

	FPrimaryAssetId GetPrimaryAssetId() const override;
	
protected:
	UPROPERTY(EditAnywhere, Category = Weapon)
	TObjectPtr<class UStaticMesh> WeaponMesh;

	UPROPERTY(EditAnywhere, Category = Stat)
	FIRCharacterStat WeaponStat;
};
