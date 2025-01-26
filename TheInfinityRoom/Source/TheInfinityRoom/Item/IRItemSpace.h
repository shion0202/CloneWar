// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/IRItemWidgetInterface.h"
#include "IRItemSpace.generated.h"

UCLASS()
class THEINFINITYROOM_API AIRItemSpace : public AActor, public IIRItemWidgetInterface
{
	GENERATED_BODY()
	
public:	
	AIRItemSpace();

	FORCEINLINE void SetItemIndex(int32 NewItemIndex) { ItemIndex = NewItemIndex; }
	FORCEINLINE class UBoxComponent* GetTrigger() { return Trigger; }
	FORCEINLINE void SetIsNoneItem(bool IsNone) { bIsNoneItem = IsNone; }

	virtual void SetupSpaceWidget(class UIRUserWidget* InUserWidget) override;

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UIRWidgetComponent> InformationWidget;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSwwep, const FHitResult& SweepHitResult);

	void LoadItems();
	void LoadNoneItem();

	UPROPERTY(VisibleAnywhere, Category = Item)
	TObjectPtr<class UBoxComponent> Trigger;

	UPROPERTY(VisibleAnywhere, Category = Item)
	TObjectPtr<class UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, Category = Item)
	TObjectPtr<class UIRItemData> Item;

	UPROPERTY(VisibleAnywhere, Category = Item)
	int32 ItemIndex = 0;

private:
	TArray<TCHAR*> Typenames = { TEXT("IRWeaponItemData"), TEXT("IRScrollItemData"), TEXT("IRPotionItemData") };
	TArray<int32> ItemPercent = { 25, 50, 25 };

	int32 bIsNoneItem : 1;
};
