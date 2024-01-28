// Fill out your copyright notice in the Description page of Project Settings.

#include "IRItemData.h"
#include "Components/StaticMeshComponent.h"

FPrimaryAssetId UIRItemData::GetPrimaryAssetId() const
{
	return FPrimaryAssetId("IRItemData", GetFName());
}
