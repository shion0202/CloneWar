// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "IRItem.h"
#include "IRUIPlayerController.generated.h"

UENUM()
enum class EWidgetType
{
	Setting,
	Help,
	Rule,
	Tutorial,
	License
};

DECLARE_DELEGATE_OneParam(FOnDisplayWidgetDelegate, EWidgetType /* InType */)
USTRUCT(BlueprintType)
struct FDisplayWidgetDelegateWrapper
{
	GENERATED_BODY()

	FDisplayWidgetDelegateWrapper() {}
	FDisplayWidgetDelegateWrapper(const FOnDisplayWidgetDelegate& InDisplayWidgetDelegate) : DisplayWidgetDelegate(InDisplayWidgetDelegate) {}

	FOnDisplayWidgetDelegate DisplayWidgetDelegate;
};

DECLARE_DELEGATE_OneParam(FOnPreviewItemDelegate, FIRItem /* InItemData */)
USTRUCT(BlueprintType)
struct FPreviewItemDelegateWrapper
{
	GENERATED_BODY()

	FPreviewItemDelegateWrapper() {}
	FPreviewItemDelegateWrapper(const FOnPreviewItemDelegate& InPreviewItemDelegate) : PreviewItemDelegate(InPreviewItemDelegate) {}

	FOnPreviewItemDelegate PreviewItemDelegate;
};

UCLASS()
class THEINFINITYROOM_API AIRUIPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AIRUIPlayerController();

	void OnDisplayWidget(EWidgetType Type);
	void SetVolumeToDefault();

	void DisplayTitleWidget();
	void DisplayShopWidget();

	void UpdateShopMoney(int32 CurrentMoneyAmount);

	UFUNCTION(BlueprintCallable)
	void SetLanguage(FString InLanguage);

	void PreviewItem(FIRItem ItemData);
	void EquipItems();
	
protected:
	virtual void BeginPlay() override;

	void SetCameraTransform(bool IsEnterShop);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UIRTitleWidget> TitleWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI)
	TObjectPtr<class UIRTitleWidget> TitleWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UIRShopWidget> ShopWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI)
	TObjectPtr<class UIRShopWidget> ShopWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI)
	TObjectPtr<class UUserWidget> UIWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UUserWidget> SettingWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UUserWidget> HelpWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UUserWidget> RuleWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UUserWidget> TutorialWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UUserWidget> LicenseWidgetClass;

protected:
	UPROPERTY()
	TArray<FDisplayWidgetDelegateWrapper> DisplayWidgetActions;

	virtual void DisplaySettingWidget(EWidgetType InType);
	virtual void DisplayHelpWidget(EWidgetType InType);
	virtual void DisplayRuleWidget(EWidgetType InType);
	virtual void DisplayTutorialWidget(EWidgetType InType);
	virtual void DisplayLicenseWidget(EWidgetType InType);

protected:
	UPROPERTY()
	TObjectPtr<class UAudioComponent> AudioComponent;

	UPROPERTY()
	TObjectPtr<class USoundCue> BGMSoundCue;

	UPROPERTY()
	TObjectPtr<class USoundMix> MasterSoundMix;

	UPROPERTY()
	TObjectPtr<class USoundClass> BGMSoundClass;

	UPROPERTY()
	TObjectPtr<class USoundClass> SESoundClass;

protected:
	virtual void PreviewSkinItem(FIRItem ItemData);
	virtual void PreviewHeadItem(FIRItem ItemData);
	virtual void PreviewBackItem(FIRItem ItemData);
	virtual void PreviewEffectItem(FIRItem ItemData);

	UPROPERTY()
	TArray<FPreviewItemDelegateWrapper> PreviewItemActions;
};
