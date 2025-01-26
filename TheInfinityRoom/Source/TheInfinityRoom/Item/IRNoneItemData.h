// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/IRItemData.h"
#include "IRNoneItemData.generated.h"

/**
 * 
 */
UCLASS()
class THEINFINITYROOM_API UIRNoneItemData : public UIRItemData
{
	GENERATED_BODY()
	
protected:
	UIRNoneItemData();

public:
	FPrimaryAssetId GetPrimaryAssetId() const override;
};
