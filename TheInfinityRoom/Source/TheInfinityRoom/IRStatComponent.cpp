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

void UIRStatComponent::SetModifierStat(const FIRCharacterStat& InModifierStat)
{
	ModifierStat = InModifierStat;
	OnStatChanged.Broadcast();
	
	const float PrevHp = CurrentHp;
	const float AdditionalHp = ModifierStat.MaxHp;
	SetHp(PrevHp + AdditionalHp);
}

void UIRStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp(NewHp, 0.f, GetTotalStat().MaxHp);
	OnHpChanged.Broadcast(CurrentHp / GetTotalStat().MaxHp);
}
