// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Item/IRItem.h"
#include "IRShopListWidget.generated.h"

UENUM()
enum class EShopState
{
	Skin,
	Head,
	Back,
	Effect
};

/**
 * 
 */
UCLASS()
class THEINFINITYROOM_API UIRShopListWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnClickSkinItem();

	UFUNCTION()
	void OnClickHeadItem();

	UFUNCTION()
	void OnClickBackItem();

	UFUNCTION()
	void OnClickEffectItem();

	void SetShopItems();
	void SetDefaultText();

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BTN_SkinItem;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BTN_HeadItem;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BTN_BackItem;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BTN_EffectItem;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TXT_SkinItem;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TXT_HeadItem;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TXT_BackItem;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TXT_EffectItem;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UListView> LV_ShopItemList;

	UPROPERTY()
	EShopState CurrentState;

	UPROPERTY()
	TArray<FIRItem> ItemTable;
};
