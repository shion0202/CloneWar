// Fill out your copyright notice in the Description page of Project Settings.

#include "IRWeaponItemData.h"
#include "Components/StaticMeshComponent.h"

UIRWeaponItemData::UIRWeaponItemData()
{
	Type = EItemType::Weapon;
}

UStaticMesh* UIRWeaponItemData::GetItemMesh()
{
	return WeaponMesh;
}

FPrimaryAssetId UIRWeaponItemData::GetPrimaryAssetId() const
{
	return FPrimaryAssetId("IRItemData", GetFName());
}
