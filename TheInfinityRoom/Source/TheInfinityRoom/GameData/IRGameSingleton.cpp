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

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_SkinItem(TEXT(
		"/Script/Engine.DataTable'/Game/Data/DT_SkinItemTable.DT_SkinItemTable'"));
	if (nullptr != DT_SkinItem.Object)
	{
		const UDataTable* DataTable = DT_SkinItem.Object;
		check(DataTable->GetRowMap().Num() > 0);

		TArray<uint8*> ValueArray;
		DataTable->GetRowMap().GenerateValueArray(ValueArray);
		Algo::Transform(ValueArray, SkinItemTable,
			[](uint8* Value)
			{
				return *reinterpret_cast<FIRItem*>(Value);
			}
		);
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_HeadItem(TEXT(
		"/Script/Engine.DataTable'/Game/Data/DT_HeadItemTable.DT_HeadItemTable'"));
	if (nullptr != DT_HeadItem.Object)
	{
		const UDataTable* DataTable = DT_HeadItem.Object;
		check(DataTable->GetRowMap().Num() > 0);

		TArray<uint8*> ValueArray;
		DataTable->GetRowMap().GenerateValueArray(ValueArray);
		Algo::Transform(ValueArray, HeadItemTable,
			[](uint8* Value)
			{
				return *reinterpret_cast<FIRItem*>(Value);
			}
		);
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_BackItem(TEXT(
		"/Script/Engine.DataTable'/Game/Data/DT_BackItemTable.DT_BackItemTable'"));
	if (nullptr != DT_BackItem.Object)
	{
		const UDataTable* DataTable = DT_BackItem.Object;
		check(DataTable->GetRowMap().Num() > 0);

		TArray<uint8*> ValueArray;
		DataTable->GetRowMap().GenerateValueArray(ValueArray);
		Algo::Transform(ValueArray, BackItemTable,
			[](uint8* Value)
			{
				return *reinterpret_cast<FIRItem*>(Value);
			}
		);
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_EffectItem(TEXT(
		"/Script/Engine.DataTable'/Game/Data/DT_EffectItemTable.DT_EffectItemTable'"));
	if (nullptr != DT_EffectItem.Object)
	{
		const UDataTable* DataTable = DT_EffectItem.Object;
		check(DataTable->GetRowMap().Num() > 0);

		TArray<uint8*> ValueArray;
		DataTable->GetRowMap().GenerateValueArray(ValueArray);
		Algo::Transform(ValueArray, EffectItemTable,
			[](uint8* Value)
			{
				return *reinterpret_cast<FIRItem*>(Value);
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
