// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "IRItemData.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Weapon = 0,
	Potion,
	Scroll,
	None
};

UCLASS()
class THEINFINITYROOM_API UIRItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
	EItemType Type;

	UPROPERTY(EditAnywhere, Category = Item)
	TSoftObjectPtr<class UStaticMesh> ItemMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Information)
	FText ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Information)
	FText ItemDescription;
};
