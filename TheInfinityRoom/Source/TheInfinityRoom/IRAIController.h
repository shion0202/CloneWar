// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "IRAIController.generated.h"

/**
 * 
 */
UCLASS()
class THEINFINITYROOM_API AIRAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AIRAIController();

	void RunAI();
	void StopAI();

protected:
	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY()
	TObjectPtr<UBlackboardData> BBAsset;

	UPROPERTY()
	TObjectPtr<UBehaviorTree> BTAsset;
};
