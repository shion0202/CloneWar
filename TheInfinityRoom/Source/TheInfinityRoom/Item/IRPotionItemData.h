// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IRItemData.h"
#include "IRPotionItemData.generated.h"

/**
 * 
 */
UCLASS()
class THEINFINITYROOM_API UIRPotionItemData : public UIRItemData
{
	GENERATED_BODY()
	
protected:
	UIRPotionItemData();

public:
	FPrimaryAssetId GetPrimaryAssetId() const override;

	FORCEINLINE float GetHealAmount() { return HealAmount; }

protected:
	UPROPERTY(EditAnywhere, Category = Stat)
	float HealAmount;
};
