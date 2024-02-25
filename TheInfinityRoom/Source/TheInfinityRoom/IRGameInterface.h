// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IRGameInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIRGameInterface : public UInterface
{
	GENERATED_BODY()
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnStageLevelChangedDelegate, int32 /* StageLevel */);
DECLARE_MULTICAST_DELEGATE(FOnGameOverDelegate);

/**
 * 
 */
class THEINFINITYROOM_API IIRGameInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void OnStageGoToNext(int32 NewStageLevel) = 0;
	virtual void OnPlayerDead() = 0;
	virtual int32 GetStageLevel() = 0;

	FOnStageLevelChangedDelegate OnStageLevelChanged;
	FOnGameOverDelegate OnGameOver;
};
