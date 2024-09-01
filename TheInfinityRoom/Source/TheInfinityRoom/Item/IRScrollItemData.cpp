// Fill out your copyright notice in the Description page of Project Settings.


#include "IRScrollItemData.h"

UIRScrollItemData::UIRScrollItemData()
{
	Type = EItemType::Scroll;
}

FPrimaryAssetId UIRScrollItemData::GetPrimaryAssetId() const
{
	return FPrimaryAssetId("IRItemData", GetFName());
}
