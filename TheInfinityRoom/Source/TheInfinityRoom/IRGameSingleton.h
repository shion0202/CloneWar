// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "IRCharacterStat.h"
#include "IRCredit.h"
#include "IRItem.h"
#include "IRGameSingleton.generated.h"

UCLASS()
class THEINFINITYROOM_API UIRGameSingleton : public UObject
{
	GENERATED_BODY()
	
public:
	UIRGameSingleton();
	static UIRGameSingleton& Get();

public:
	FORCEINLINE FIRCharacterStat GetCharacterStat(int32 InLevel) { return CharacterStatTable.IsValidIndex(InLevel - 1) ? CharacterStatTable[InLevel - 1] : FIRCharacterStat(); }
	FORCEINLINE FIRCharacterStat GetCharacterMaxStat() { return CharacterMaxStat; }
	FORCEINLINE TArray<FIRCredit> GetCredit() { return CreditTable; }
	FORCEINLINE TArray<FIRItem> GetSkinItems() { return SkinItemTable; }
	FORCEINLINE TArray<FIRItem> GetHeadItems() { return HeadItemTable; }
	FORCEINLINE TArray<FIRItem> GetBackItems() { return BackItemTable; }
	FORCEINLINE TArray<FIRItem> GetEffectItems() { return EffectItemTable; }

	UPROPERTY()
	int32 CharacterMaxLevel;

private:
	TArray<FIRCharacterStat> CharacterStatTable;
	FIRCharacterStat CharacterMaxStat;
	TArray<FIRCredit> CreditTable;

	TArray<FIRItem> SkinItemTable;
	TArray<FIRItem> HeadItemTable;
	TArray<FIRItem> BackItemTable;
	TArray<FIRItem> EffectItemTable;
};
