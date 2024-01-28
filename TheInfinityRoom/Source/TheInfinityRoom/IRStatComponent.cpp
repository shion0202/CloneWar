// Fill out your copyright notice in the Description page of Project Settings.

#include "IRStatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "IRGameSingleton.h"

UIRStatComponent::UIRStatComponent()
{
	Level = 1;
}

void UIRStatComponent::BeginPlay()
{
	Super::BeginPlay();

	SetLevel(Level);
	SetHp(GetTotalStat().MaxHp);
}

void UIRStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UIRStatComponent::SetLevel(int32 NewLevel)
{
	Level = FMath::Clamp(NewLevel, 1, UIRGameSingleton::Get().CharacterMaxLevel);
	BaseStat = UIRGameSingleton::Get().GetCharacterStat(NewLevel);
	TotalStat = BaseStat;
}

float UIRStatComponent::ApplyDamage(float InDamage)
{
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp(InDamage, 0.f, InDamage);

	SetHp(PrevHp - ActualDamage);
	if (CurrentHp < KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
	}

	return ActualDamage;
}

void UIRStatComponent::SetWeaponStat(const FIRCharacterStat& InWeaponStat)
{
	WeaponStat = InWeaponStat;
	CalculateTotalStat();
}

void UIRStatComponent::AddScrollStat(const FIRCharacterStat& InScrollStat)
{
	ScrollStats.Add(InScrollStat);
	CalculateTotalStat();
}

void UIRStatComponent::CalculateTotalStat()
{
	FIRCharacterStat NewWeaponStat = BaseStat + WeaponStat;
	for (const FIRCharacterStat& ScrollStat : ScrollStats)
	{
		NewWeaponStat = NewWeaponStat + ScrollStat;
	}

	TotalStat = NewWeaponStat;
	OnStatChanged.Broadcast(TotalStat);
	OnHpChanged.Broadcast(TotalStat.MaxHp, CurrentHp);
}

void UIRStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp(NewHp, 0.f, TotalStat.MaxHp);
	OnHpChanged.Broadcast(TotalStat.MaxHp, CurrentHp);
}
