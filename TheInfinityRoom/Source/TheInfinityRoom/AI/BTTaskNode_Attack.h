// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_Attack.generated.h"

/**
 * 
 */
UCLASS()
class THEINFINITYROOM_API UBTTaskNode_Attack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTaskNode_Attack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
