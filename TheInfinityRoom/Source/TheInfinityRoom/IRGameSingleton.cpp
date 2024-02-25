// Fill out your copyright notice in the Description page of Project Settings.

#include "IRGameSingleton.h"

UIRGameSingleton::UIRGameSingleton()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Stat(TEXT(
		"/Script/Engine.DataTable'/Game/Data/DT_CharacterStatTable.DT_CharacterStatTable'"));
	if (nullptr != DT_Stat.Object)
	{
		const UDataTable* DataTable = DT_Stat.Object;
		check(DataTable->GetRowMap().Num() > 0);

		TArray<uint8*> ValueArray;
		DataTable->GetRowMap().GenerateValueArray(ValueArray);
		Algo::Transform(ValueArray, CharacterStatTable,
			[](uint8* Value)
			{
				return *reinterpret_cast<FIRCharacterStat*>(Value);
			}
		);
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_MaxStat(TEXT(
		"/Script/Engine.DataTable'/Game/Data/DT_CharacterMaxStatTable.DT_CharacterMaxStatTable'"));
	if (nullptr != DT_MaxStat.Object)
	{
		const UDataTable* DataTable = DT_MaxStat.Object;
		check(DataTable->GetRowMap().Num() > 0);

		TArray<uint8*> ValueArray;
		DataTable->GetRowMap().GenerateValueArray(ValueArray);
		CharacterMaxStat = *reinterpret_cast<FIRCharacterStat*>(ValueArray[0]);
	}

	CharacterMaxLevel = CharacterStatTable.Num();
	ensure(CharacterMaxLevel > 0);

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Credit(TEXT(
		"/Script/Engine.DataTable'/Game/Data/DT_CreditTable.DT_CreditTable'"));
	if (nullptr != DT_Credit.Object)
	{
		const UDataTable* DataTable = DT_Credit.Object;
		check(DataTable->GetRowMap().Num() > 0);

		TArray<uint8*> ValueArray;
		DataTable->GetRowMap().GenerateValueArray(ValueArray);
		Algo::Transform(ValueArray, CreditTable,
			[](uint8* Value)
			{
				return *reinterpret_cast<FIRCredit*>(Value);
			}
		);
	}
}

UIRGameSingleton& UIRGameSingleton::Get()
{
	UIRGameSingleton* Singleton = CastChecked<UIRGameSingleton>(GEngine->GameSingleton);
	if (Singleton)
	{
		return *Singleton;
	}

	return *NewObject<UIRGameSingleton>();
}
