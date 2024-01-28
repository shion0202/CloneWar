// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IRItemData.h"
#include "IRCharacterStat.h"
#include "IRScrollItemData.generated.h"

UCLASS()
class THEINFINITYROOM_API UIRScrollItemData : public UIRItemData
{
	GENERATED_BODY()
	
protected:
	UIRScrollItemData();

public:
	FPrimaryAssetId GetPrimaryAssetId() const override;

	FORCEINLINE FIRCharacterStat GetScrollStat() { return ScrollStat; }

protected:
	UPROPERTY(EditAnywhere, Category = Stat)
	FIRCharacterStat ScrollStat;
};
