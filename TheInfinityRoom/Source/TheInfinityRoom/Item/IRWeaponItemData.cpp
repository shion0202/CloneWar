// Fill out your copyright notice in the Description page of Project Settings.

#include "IRWeaponItemData.h"
#include "Components/StaticMeshComponent.h"

UIRWeaponItemData::UIRWeaponItemData()
{
	Type = EItemType::Weapon;
}

FPrimaryAssetId UIRWeaponItemData::GetPrimaryAssetId() const
{
	return FPrimaryAssetId("IRWeaponItemData", GetFName());
}
