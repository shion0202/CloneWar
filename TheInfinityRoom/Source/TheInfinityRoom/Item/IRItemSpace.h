// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IRItemSpace.generated.h"

UCLASS()
class THEINFINITYROOM_API AIRItemSpace : public AActor
{
	GENERATED_BODY()
	
public:	
	AIRItemSpace();

	FORCEINLINE void SetItemIndex(int32 NewItemIndex) { ItemIndex = NewItemIndex; }
	FORCEINLINE class UBoxComponent* GetTrigger() { return Trigger; }

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, Category = Item)
	TObjectPtr<class UBoxComponent> Trigger;

	UPROPERTY(VisibleAnywhere, Category = Item)
	TObjectPtr<class UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, Category = Item)
	TObjectPtr<class UIRItemData> Item;

	UPROPERTY(VisibleAnywhere, Category = Item)
	int32 ItemIndex = 0;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSwwep, const FHitResult& SweepHitResult);
};
