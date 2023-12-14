// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "IRGameInstance.generated.h"

USTRUCT()
struct FIRCharacterData : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Attack;
};

UCLASS()
class THEINFINITYROOM_API UIRGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UIRGameInstance();

	FIRCharacterData* GetStatData(int32 InLevel);

private:
	UPROPERTY()
	TObjectPtr<class UDataTable> Stats;
};
