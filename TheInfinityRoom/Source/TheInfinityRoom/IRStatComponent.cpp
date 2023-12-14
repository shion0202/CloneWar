// Fill out your copyright notice in the Description page of Project Settings.

#include "IRStatComponent.h"
#include "IRGameInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UIRStatComponent::UIRStatComponent()
{
	bWantsInitializeComponent = true;
	Level = 1;
}


// Called when the game starts
void UIRStatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UIRStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
	SetLevel(Level);
}

void UIRStatComponent::SetLevel(int32 NewLevel)
{
	UIRGameInstance* GameInstance = Cast<UIRGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
		auto StatData = GameInstance->GetStatData(NewLevel);
		Level = StatData->Level;
		MaxHp = StatData->MaxHp;
		Attack = StatData->Attack;
		SetHp(MaxHp);
	}
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

void UIRStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp(NewHp, 0.f, MaxHp);
	OnHpChanged.Broadcast(CurrentHp / MaxHp);
}
