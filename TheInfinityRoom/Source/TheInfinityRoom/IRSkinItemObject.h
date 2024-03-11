// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "IRSkinItem.h"
#include "IRSkinItemObject.generated.h"

/**
 * 
 */
UCLASS()
class THEINFINITYROOM_API UIRSkinItemObject : public UObject
{
	GENERATED_BODY()
	
public:
	UIRSkinItemObject();

	FORCEINLINE FIRSkinItem GetSkinItemData() { return SkinItemData; }
	FORCEINLINE void SetSkinItemData(FIRSkinItem InItemData) { SkinItemData = InItemData; }
	FORCEINLINE bool IsPurchased() { return bIsPurchased; }
	FORCEINLINE void SetIsPurchased(bool InIsPurchased) { bIsPurchased = InIsPurchased; }

protected:
	UPROPERTY()
	FIRSkinItem SkinItemData;

	UPROPERTY()
	uint8 bIsPurchased : 1;
};
