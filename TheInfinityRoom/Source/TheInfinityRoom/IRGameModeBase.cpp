// Fill out your copyright notice in the Description page of Project Settings.

#include "IRGameModeBase.h"
#include "IRPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "IRSaveGame.h"

AIRGameModeBase::AIRGameModeBase()
{
	static ConstructorHelpers::FClassFinder<APawn> PawnRef(
		TEXT("/Script/CoreUObject.Class'/Script/TheInfinityRoom.IRCharacterPlayer'"));
	if (PawnRef.Succeeded())
	{
		DefaultPawnClass = PawnRef.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(
		TEXT("/Script/CoreUObject.Class'/Script/TheInfinityRoom.IRPlayerController'"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}

	StageLevel = 1;
}

void AIRGameModeBase::SetVolumeData(float InBGMValue, float InSEValue)
{
	SaveGameInstance->BGMVolumeValue = InBGMValue;
	SaveGameInstance->SEVolumeValue = InSEValue;
	SaveGameData();
}

void AIRGameModeBase::SetLanguageData(FString InLanguage)
{
	SaveGameInstance->CurrentLanguage = InLanguage;
	SaveGameData();
}

void AIRGameModeBase::SaveGameData()
{
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("Player0"), 0);
}

void AIRGameModeBase::OnStageGoToNext(int32 NewStageLevel)
{
	StageLevel = NewStageLevel;
	OnStageLevelChanged.Broadcast(StageLevel);
}

void AIRGameModeBase::OnReturnReward(int32 NewRewardAmount)
{
	RecentRewardAmount = NewRewardAmount;
	SaveGameInstance->MoneyAmount += RecentRewardAmount;
	SaveGameData();
}

void AIRGameModeBase::OnPlayerDead()
{
	// For single play.
	AIRPlayerController* PlayerController = Cast<AIRPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		PlayerController->GameOver();
	}

	OnGameOver.Broadcast();

	
}

int32 AIRGameModeBase::GetStageLevel()
{
	return StageLevel;
}

int32 AIRGameModeBase::GetRewardAmount()
{
	return RecentRewardAmount;
}

void AIRGameModeBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SaveGameInstance = Cast<UIRSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("Player0"), 0));
	if (!SaveGameInstance)
	{
		SaveGameInstance = NewObject<UIRSaveGame>();
	}
}