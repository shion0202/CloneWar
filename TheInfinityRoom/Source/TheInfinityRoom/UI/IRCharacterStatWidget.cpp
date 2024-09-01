// Fill out your copyright notice in the Description page of Project Settings.

#include "IRCharacterStatWidget.h"
#include "Components/ProgressBar.h"
#include "GameData/IRGameSingleton.h"

void UIRCharacterStatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	FIRCharacterStat MinLevelStat = UIRGameSingleton::Get().GetCharacterStat(1);
	FIRCharacterStat MaxLevelStat = UIRGameSingleton::Get().GetCharacterMaxStat();

	for (TFieldIterator<FNumericProperty> PropIt(FIRCharacterStat::StaticStruct()); PropIt; ++PropIt)
	{
		const FName PropKey(PropIt->GetName());
		const FName ProgressBarControlName = *FString::Printf(TEXT("PB_%sValue"), *PropIt->GetName());

		UProgressBar* ProgressBar = Cast<UProgressBar>(GetWidgetFromName(ProgressBarControlName));
		if (ProgressBar)
		{
			ProgressBarLookup.Add(PropKey, ProgressBar);

			float StatData = 0.f;
			PropIt->GetValue_InContainer((const void*)&MinLevelStat, &StatData);
			MinLevelStatValues.Add(PropKey, StatData);
			PropIt->GetValue_InContainer((const void*)&MaxLevelStat, &StatData);
			MaxLevelStatValues.Add(PropKey, StatData);
		}
	}
}

void UIRCharacterStatWidget::UpdateStat(const FIRCharacterStat& TotalStat)
{
	for (TFieldIterator<FNumericProperty> PropIt(FIRCharacterStat::StaticStruct()); PropIt; ++PropIt)
	{
		const FName PropKey(PropIt->GetName());

		float StatData = 0.f;
		PropIt->GetValue_InContainer((const void*)&TotalStat, &StatData);

		UProgressBar** ProgressBarPtr = ProgressBarLookup.Find(PropKey);
		float* MinLevelStatValue = MinLevelStatValues.Find(PropKey);
		float* MaxLevelStatValue = MaxLevelStatValues.Find(PropKey);
		if (ProgressBarPtr && MaxLevelStatValue)
		{
			(*ProgressBarPtr)->SetPercent((StatData - (*MinLevelStatValue)) / ((*MaxLevelStatValue) - (*MinLevelStatValue)));
		}
	}
}
