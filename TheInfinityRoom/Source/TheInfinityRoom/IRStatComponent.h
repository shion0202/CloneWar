// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IRCharacterStat.h"
#include "IRStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHpChangedDelegate, float /* MaxHp */, float /* CurrentHp */);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnStatChangedDelegate, const FIRCharacterStat& /* TotalStat */);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THEINFINITYROOM_API UIRStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UIRStatComponent();

protected:
	virtual void InitializeComponent() override;

public:
	FOnHpZeroDelegate OnHpZero;
	FOnHpChangedDelegate OnHpChanged;
	FOnStatChangedDelegate OnStatChanged;

	void SetLevel(int32 NewLevel);
	float ApplyDamage(float InDamage);
	void SetBaseStat(const FIRCharacterStat& InBaseStat);
	void SetWeaponStat(const FIRCharacterStat& InWeaponStat);
	void AddScrollStat(const FIRCharacterStat& InScrollStat);
	void CalculateTotalStat();
	FORCEINLINE void HealHp(float InHealAmount) { SetHp(CurrentHp + InHealAmount); }

	FORCEINLINE FIRCharacterStat GetTotalStat() const { return TotalStat; }
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
	FIRCharacterStat TotalStat;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	FIRCharacterStat BaseStat;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	FIRCharacterStat WeaponStat;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	TArray<FIRCharacterStat> ScrollStats;
};
