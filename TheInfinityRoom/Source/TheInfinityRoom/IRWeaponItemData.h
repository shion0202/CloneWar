// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IRItemData.h"
#include "IRWeaponItemData.generated.h"

UCLASS()
class THEINFINITYROOM_API UIRWeaponItemData : public UIRItemData
{
	GENERATED_BODY()
	
protected:
	UIRWeaponItemData();
	
public:
	UPROPERTY(EditAnywhere, Category = Weapon)
	TObjectPtr<USkeletalMesh> WeaponMesh;
};
