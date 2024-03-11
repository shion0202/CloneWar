// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "IRSkinItem.h"
#include "IRListShopItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class THEINFINITYROOM_API UIRListShopItemWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnPreviewClick();

	UFUNCTION()
	void OnApplyClick();

	void UpdateButton();
	void PreviewMesh();

protected:
	UPROPERTY()
	TObjectPtr<class UIRSkinItemObject> Item;

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
};
