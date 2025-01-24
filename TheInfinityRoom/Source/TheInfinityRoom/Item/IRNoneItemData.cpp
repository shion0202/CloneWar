// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/IRNoneItemData.h"

UIRNoneItemData::UIRNoneItemData()
{
	Type = EItemType::None;
}

FPrimaryAssetId UIRNoneItemData::GetPrimaryAssetId() const
{
	return FPrimaryAssetId("IRNoneItemData", GetFName());
}
