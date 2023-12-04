// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IRCharacter.h"
#include "IRCharacterNonPlayer.generated.h"

/**
 * 
 */
UCLASS()
class THEINFINITYROOM_API AIRCharacterNonPlayer : public AIRCharacter
{
	GENERATED_BODY()
	
public:
	AIRCharacterNonPlayer();

protected:
	void SetDead() override;
};
