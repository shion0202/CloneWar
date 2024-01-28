// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "IRComboData.generated.h"

UCLASS()
class THEINFINITYROOM_API UIRComboData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UIRComboData();

	UPROPERTY(EditAnywhere, Category = Name)
	FString MontageSectionName;

	UPROPERTY(EditAnywhere, Category = Name)
	uint8 MaxComboCount;

	UPROPERTY(EditAnywhere, Category = Name)
	float FrameRate;

	UPROPERTY(EditAnywhere, Category = ComboData)
	TArray<float> EffectiveFrameCount;
};
