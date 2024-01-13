// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IRCharacterStat.h"
#include "IRStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /* HpRatio */);
DECLARE_MULTICAST_DELEGATE(FOnStatChangedDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THEINFINITYROOM_API UIRStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UIRStatComponent();

protected:
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

public:
	FOnHpZeroDelegate OnHpZero;
	FOnHpChangedDelegate OnHpChanged;
	FOnStatChangedDelegate OnStatChanged;

	void SetLevel(int32 NewLevel);
	float ApplyDamage(float InDamage);
	void SetModifierStat(const FIRCharacterStat& InModifierStat);

	FORCEINLINE FIRCharacterStat GetTotalStat() const { return BaseStat + ModifierStat; }
	FORCEINLINE int32 GetLevel() const { return Level; }
	FORCEINLINE float GetCurrentHp() const { return CurrentHp; }

protected:
	void SetHp(float NewHp);
	
private:
	UPROPERTY(VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 Level;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float CurrentHp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	FIRCharacterStat BaseStat;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	FIRCharacterStat ModifierStat;
};
