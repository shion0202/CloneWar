// Fill out your copyright notice in the Description page of Project Settings.

#include "IRGameInstance.h"

UIRGameInstance::UIRGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DT(TEXT(
		"/Script/Engine.DataTable'/Game/Data/StatTable.StatTable'"));
	if (DT.Succeeded())
	{
		// You can make crash if DT failed.
		Stats = DT.Object;
	}
}

FIRCharacterData* UIRGameInstance::GetStatData(int32 InLevel)
{
	return Stats->FindRow<FIRCharacterData>(*FString::FromInt(InLevel), TEXT(""));
}
