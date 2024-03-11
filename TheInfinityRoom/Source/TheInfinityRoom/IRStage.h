// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IRStage.generated.h"

DECLARE_DELEGATE(FOnStageChangedDelegate);

USTRUCT(BlueprintType)
struct FStageChangedDelegateWrapper
{
	GENERATED_BODY()

	FStageChangedDelegateWrapper() {}
	FStageChangedDelegateWrapper(const FOnStageChangedDelegate& InDelegate) : StageChangeDelegate(InDelegate) {}

	FOnStageChangedDelegate StageChangeDelegate;
};

UENUM(BlueprintType)
enum class EStageState : uint8
{
	READY,
	BATTLE,
	REWARD
};

UCLASS()
class THEINFINITYROOM_API AIRStage : public AActor
{
	GENERATED_BODY()
	
public:	
	AIRStage();

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, Category = Stage, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UStaticMeshComponent> StageMesh;

protected:
	UPROPERTY(EditAnywhere, Category = Stage, Meta = (AllowPrivateAccess = true))
	EStageState CurrentState;

	UPROPERTY()
	TMap<EStageState, FStageChangedDelegateWrapper> StateChangeActions;

	void SetState(EStageState NewState);

	void SetReady();
	void SetStartBattle();
	void SetChooseReward();

protected:
	UPROPERTY(EditAnywhere, Category = Battle, Meta = (AllowPrivateAccess = true))
	float PreparationTime;

	FTimerHandle ReadyTimeHandle;

	void OnEndPreparationTime();

protected:
	UPROPERTY(EditAnywhere, Category = Battle, Meta = (AllowPrivateAccess = true))
	TSubclassOf<class AIRCharacterNonPlayer> EnemyClass;

	UPROPERTY(EditAnywhere, Category = Battle, Meta = (AllowPrivateAccess = true))
	float EnemySpawnTime;

	UPROPERTY(VisibleAnywhere, Category = Battle, Meta = (AllowPrivateAccess = true))
	int32 TargetEnemyCount;

	int32 CurrentEnemyCount;
	int32 DestroyEnemyCount;

	FORCEINLINE int32 GetTargetEnemyCount() { return TargetEnemyCount; }
	FORCEINLINE void SetTargetEnemyCount(int32 NewCount) { TargetEnemyCount = NewCount; }

	UFUNCTION()
	void OnEnemyDestroyed(AActor* DestroyedActor);

	FTimerHandle EnemyTimeHandle;
	void OnEnemySpawn();

protected:
	UPROPERTY(VisibleAnywhere, Category = Reward, Meta = (AllowPrivateAccess = true))
	TSubclassOf<class AIRItemSpace> RewardClass;

	UPROPERTY(VisibleAnywhere, Category = Reward, Meta = (AllowPrivateAccess = true))
	TArray<TWeakObjectPtr<class AIRItemSpace>> Rewards;

	TArray<FVector> RewardLocations;

	UFUNCTION()
	void OnRewardTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSwwep, const FHitResult& SweepHitResult);

	void SpawnRewards();

protected:
	UPROPERTY(VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 CurrentStageLevel;

	UPROPERTY(VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 CurrentRewardAmount;

public:
	FORCEINLINE int32 GetStageLevel() { return CurrentStageLevel; }
	FORCEINLINE void SetStageLevel(int32 NewStageLevel) { CurrentStageLevel = NewStageLevel; }

protected:
	void StopBGMMusic();
	void PlayGameOverMusic();

	UPROPERTY()
	TObjectPtr<class UAudioComponent> AudioComponent;

	UPROPERTY()
	TObjectPtr<class USoundCue> BGMSoundCue;

	UPROPERTY()
	TObjectPtr<class USoundCue> GameOverSoundCue;
};
