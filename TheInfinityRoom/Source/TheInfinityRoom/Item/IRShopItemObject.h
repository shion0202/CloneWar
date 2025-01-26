// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "IRItem.h"
#include "IRShopItemObject.generated.h"

/**
 * 
 */
UCLASS()
class THEINFINITYROOM_API UIRShopItemObject : public UObject
{
	GENERATED_BODY()
	
public:
	UIRShopItemObject();

	FORCEINLINE FIRItem GetItemData() { return ItemData; }
	FORCEINLINE void SetItemData(FIRItem InItemData) { ItemData = InItemData; }

	FORCEINLINE bool IsPurchased() { return bIsPurchased; }
	FORCEINLINE void SetIsPurchased(bool InIsPurchased) { bIsPurchased = InIsPurchased; }

	FORCEINLINE bool IsEquiped() { return bIsEquiped; }
	FORCEINLINE void SetIsEquiped(bool InIsEquiped) { bIsEquiped = InIsEquiped; }

protected:
	UPROPERTY()
	FIRItem ItemData;

	UPROPERTY()
	uint8 bIsPurchased : 1;

	UPROPERTY()
	uint8 bIsEquiped : 1;
};
