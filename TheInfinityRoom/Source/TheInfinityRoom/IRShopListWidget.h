// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IRShopListWidget.generated.h"

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

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BTN_SkinItem;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UListView> LV_ShopItemList;
};
