// Fill out your copyright notice in the Description page of Project Settings.

#include "IRGameModeBase.h"
#include "Player/IRPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Player/IRSaveGame.h"

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

void AIRGameModeBase::SetSensitivityData(float InSensitivity)
{
	SaveGameInstance->MouseSensitivity = InSensitivity;
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

void AIRGameModeBase::OnDeliverEnemyCount(int32 DestroyEnemyCount, int32 TargetEnemyCount)
{
	OnEnemyCountChanged.Broadcast(DestroyEnemyCount, TargetEnemyCount);
}

void AIRGameModeBase::OnChangeObjective(bool IsReady)
{
	OnObjectiveChanged.Broadcast(IsReady);
}

void AIRGameModeBase::OnReturnReward(int32 NewRewardAmount)
{
	RecentRewardAmount = NewRewardAmount;
	SaveGameInstance->MoneyAmount += RecentRewardAmount;
	SaveGameData();

	GetMoney(RecentRewardAmount);
}

void AIRGameModeBase::OnPlayerDead()
{
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

void AIRGameModeBase::ClearStage(int32 InClearedStage)
{
	AIRPlayerController* PlayerController = Cast<AIRPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		PlayerController->ClearStage(InClearedStage);
	}
}

void AIRGameModeBase::KillEnemy(int32 InEnemyAmount)
{
	AIRPlayerController* PlayerController = Cast<AIRPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		PlayerController->KillEnemy(InEnemyAmount);
	}
}

void AIRGameModeBase::GetMoney(int32 InMoneyAmount)
{
	AIRPlayerController* PlayerController = Cast<AIRPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		PlayerController->GetMoney(InMoneyAmount);
	}
}

void AIRGameModeBase::CountJump()
{
	AIRPlayerController* PlayerController = Cast<AIRPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		PlayerController->CountJump();
	}
}

void AIRGameModeBase::UploadStageLevel()
{
	AIRPlayerController* PlayerController = Cast<AIRPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		PlayerController->UploadStageLevel(StageLevel);
	}
}

void AIRGameModeBase::UploadHealAmount(int32 InHealAmount)
{
	AIRPlayerController* PlayerController = Cast<AIRPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		PlayerController->UploadHealAmount(InHealAmount);
	}
}

void AIRGameModeBase::UploadNewGameCount()
{
	AIRPlayerController* PlayerController = Cast<AIRPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		PlayerController->UploadNewGameCount();
	}
}

void AIRGameModeBase::ClearSplendorAchievements(int32 InClearedStage)
{
	AIRPlayerController* PlayerController = Cast<AIRPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		PlayerController->ClearSplendorAchievements(InClearedStage);
	}
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
