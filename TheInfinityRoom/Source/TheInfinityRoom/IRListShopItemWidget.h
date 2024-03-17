// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "IRListShopItemWidget.generated.h"

DECLARE_DELEGATE(FOnEquipItemDelegate)
USTRUCT(BlueprintType)
struct FFOnEquipItemDelegateWrapper
{
	GENERATED_BODY()

	FFOnEquipItemDelegateWrapper() {}
	FFOnEquipItemDelegateWrapper(const FOnEquipItemDelegate& InEquipItemDelegate) : EquipItemDelegate(InEquipItemDelegate) {}

	FOnEquipItemDelegate EquipItemDelegate;
};

/**
 * 
 */
UCLASS()
class THEINFINITYROOM_API UIRListShopItemWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	void OnEquipItem();

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnPreviewClick();

	UFUNCTION()
	void OnApplyClick();

	void UpdateButton();
	void PreviewItem();

protected:
	UPROPERTY()
	TObjectPtr<class UIRShopItemObject> Item;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TXT_ItemName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> IMG_ItemIcon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BTN_Preview;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BTN_Apply;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TXT_Apply;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TXT_PurchaseFront;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TXT_PurchaseBack;

protected:
	virtual void EquipSkinItem();
	virtual void EquipHeadItem();
	virtual void EquipBackItem();
	virtual void EquipEffectItem();

	UPROPERTY()
	TArray<FFOnEquipItemDelegateWrapper> EquipItemActions;
};
