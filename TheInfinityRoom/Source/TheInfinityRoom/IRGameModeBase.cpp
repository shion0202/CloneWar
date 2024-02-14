// Fill out your copyright notice in the Description page of Project Settings.

#include "IRGameModeBase.h"
#include "IRPlayerController.h"

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

void AIRGameModeBase::OnStageGoToNext(int32 NewStageLevel)
{
	StageLevel = NewStageLevel;
	OnStageLevelChanged.Broadcast(StageLevel);
}

void AIRGameModeBase::OnPlayerDead()
{
	// For single play.
	AIRPlayerController* PlayerController = Cast<AIRPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		PlayerController->GameOver();
	}
}

int32 AIRGameModeBase::GetStageLevel()
{
	return StageLevel;
}
