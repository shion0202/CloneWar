// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IRCharacter.h"
#include "Interface/IRCharacterAIInterface.h"
#include "IRCharacterNonPlayer.generated.h"

UCLASS()
class THEINFINITYROOM_API AIRCharacterNonPlayer : public AIRCharacter, public IIRCharacterAIInterface
{
	GENERATED_BODY()
	
public:
	AIRCharacterNonPlayer();

protected:
	virtual void SetDead() override;

	virtual float GetAIPatrolRadius() override;
	virtual float GetAIDetectRange() override;
	virtual float GetAIAttackRange() override;
	virtual float GetAITurnSpeed() override;

	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) override;
	virtual void AttackByAI() override;

	virtual bool GetIsHitting() override;

	FAICharacterAttackFinished OnAttackFinished;

	virtual void NotifyComboEnd() override;
};
