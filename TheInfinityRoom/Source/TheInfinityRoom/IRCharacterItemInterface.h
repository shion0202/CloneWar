// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IRCharacterItemInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIRCharacterItemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class THEINFINITYROOM_API IIRCharacterItemInterface
{
	GENERATED_BODY()

public:
	virtual void TakeItem(class UIRItemData* InItemData) = 0;
};
