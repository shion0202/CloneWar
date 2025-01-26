// Fill out your copyright notice in the Description page of Project Settings.


#include "IRPotionItemData.h"

UIRPotionItemData::UIRPotionItemData()
{
	Type = EItemType::Potion;
}

FPrimaryAssetId UIRPotionItemData::GetPrimaryAssetId() const
{
	return FPrimaryAssetId("IRPotionItemData", GetFName());
}
