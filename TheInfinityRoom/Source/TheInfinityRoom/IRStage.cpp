// Fill out your copyright notice in the Description page of Project Settings.

#include "IRStage.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "IRCharacterNonPlayer.h"
#include "IRItemSpace.h"
#include "IRGameSingleton.h"

AIRStage::AIRStage()
{
	StageMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StageMesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM(TEXT(
		"/Script/Engine.StaticMesh'/Game/Maps/_GENERATED/user/Box_4499228E.Box_4499228E'"));
	if (SM.Succeeded())
	{
		StageMesh->SetStaticMesh(SM.Object);
	}

	RootComponent = StageMesh;
	StageMesh->SetCollisionProfileName(TEXT("NoCollision"));

	CurrentState = EStageState::READY;
	PreparationTime = 1.f;

	EnemyClass = AIRCharacterNonPlayer::StaticClass();
	EnemySpawnTime = 1.f;
	TargetEnemyCount = 1;
	CurrentEnemyCount = 0;
	DestroyEnemyCount = 0;

	RewardClass = AIRItemSpace::StaticClass();
	FVector RewardLocation = GetActorLocation() + FVector(700.f, 300.f, 170.f);
	RewardLocations.Add(RewardLocation);
	RewardLocation = GetActorLocation() + FVector(700.f, -300.f, 170.f);
	RewardLocations.Add(RewardLocation);

	CurrentStageLevel = 1;
}

void AIRStage::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	StateChangeActions.Add(EStageState::READY,
		FStageChangedDelegateWrapper(FOnStageChangedDelegate::CreateUObject(this, &AIRStage::SetReady)));
	StateChangeActions.Add(EStageState::BATTLE,
		FStageChangedDelegateWrapper(FOnStageChangedDelegate::CreateUObject(this, &AIRStage::SetStartBattle)));
	StateChangeActions.Add(EStageState::REWARD,
		FStageChangedDelegateWrapper(FOnStageChangedDelegate::CreateUObject(this, &AIRStage::SetChooseReward)));
}

void AIRStage::BeginPlay()
{
	Super::BeginPlay();

	SetState(EStageState::READY);
}

void AIRStage::SetState(EStageState NewState)
{
	CurrentState = NewState;
	if (StateChangeActions.Contains(CurrentState))
	{
		StateChangeActions[CurrentState].StageChangeDelegate.ExecuteIfBound();
	}
}

void AIRStage::SetReady()
{
	GetWorld()->GetTimerManager().SetTimer(ReadyTimeHandle, this, &AIRStage::OnEndPreparationTime, PreparationTime, false);
}

void AIRStage::SetStartBattle()
{
	GetWorld()->GetTimerManager().SetTimer(EnemyTimeHandle, this, &AIRStage::OnEnemySpawn, EnemySpawnTime, true);
}

void AIRStage::SetChooseReward()
{
	SpawnRewards();
}

void AIRStage::OnEndPreparationTime()
{
	SetState(EStageState::BATTLE);
}

void AIRStage::OnEnemyDestroyed(AActor* DestroyedActor)
{
	++DestroyEnemyCount;
	if (DestroyEnemyCount >= CurrentEnemyCount)
	{
		CurrentEnemyCount = 0;
		DestroyEnemyCount = 0;
		SetState(EStageState::REWARD);
	}
}

void AIRStage::OnEnemySpawn()
{
	const FTransform SpawnTransform(GetActorLocation() + FVector(FMath::RandRange(-800.f, 800.f), FMath::RandRange(-800.f, 800.f), 255.f));
	AIRCharacterNonPlayer* EnemyCharacter = GetWorld()->SpawnActorDeferred<AIRCharacterNonPlayer>(EnemyClass, SpawnTransform);
	if (EnemyCharacter)
	{
		EnemyCharacter->SetLevel(CurrentStageLevel);
		EnemyCharacter->OnDestroyed.AddDynamic(this, &AIRStage::OnEnemyDestroyed);
		EnemyCharacter->FinishSpawning(SpawnTransform);
	}

	++CurrentEnemyCount;
	if (CurrentEnemyCount >= TargetEnemyCount)
	{
		GetWorld()->GetTimerManager().ClearTimer(EnemyTimeHandle);
	}
}

void AIRStage::OnRewardTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSwwep, const FHitResult& SweepHitResult)
{
	for (const auto& Reward : Rewards)
	{
		if (Reward.IsValid())
		{
			AIRItemSpace* ValidItemSpace = Reward.Get();
			AActor* OverlappedReward = OverlappedComponent->GetOwner();
			if (OverlappedReward != ValidItemSpace)
			{
				ValidItemSpace->Destroy();
			}
		}
	}

	Rewards.Empty();

	int32 RandomIndex = FMath::RandRange(0, 99);
	if (RandomIndex < 30)
	{
		SetStageLevel(FMath::Clamp(CurrentStageLevel + 1, 1, UIRGameSingleton::Get().CharacterMaxLevel));
	}
	else
	{
		SetTargetEnemyCount(FMath::Clamp(TargetEnemyCount + 1, 1, TargetEnemyCount + 1));
	}

	SetState(EStageState::READY);
}

void AIRStage::SpawnRewards()
{
	for (const auto& RewardLocation : RewardLocations)
	{
		FTransform WorldSpawnTransform(RewardLocation);
		AIRItemSpace* ItemSpaceActor = GetWorld()->SpawnActorDeferred<AIRItemSpace>(RewardClass, WorldSpawnTransform);
		if (ItemSpaceActor)
		{
			ItemSpaceActor->GetTrigger()->OnComponentBeginOverlap.AddDynamic(this, &AIRStage::OnRewardTriggerBeginOverlap);
			Rewards.Add(ItemSpaceActor);
		}
	}

	for (const auto& Reward : Rewards)
	{
		if (Reward.IsValid())
		{
			Reward.Get()->FinishSpawning(Reward.Get()->GetActorTransform());
		}
	}
}
