// Fill out your copyright notice in the Description page of Project Settings.

#include "IRCharacterNonPlayer.h"
#include "IRAIController.h"
#include "IRStatComponent.h"

AIRCharacterNonPlayer::AIRCharacterNonPlayer()
{
	AIControllerClass = AIRAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bIsPlayer = false;
}

void AIRCharacterNonPlayer::SetDead()
{
	Super::SetDead();

	AIRAIController* AIController = Cast<AIRAIController>(GetController());
	if (AIController)
	{
		AIController->StopAI();
	}

	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			Destroy();
		}
	), 3.f, false);
}

float AIRCharacterNonPlayer::GetAIPatrolRadius()
{
	return 1500.f;
}

float AIRCharacterNonPlayer::GetAIDetectRange()
{
	return 1000.f;
}

float AIRCharacterNonPlayer::GetAIAttackRange()
{
	return Stat->GetTotalStat().AttackRange + Stat->GetTotalStat().AttackRadius * 2;
}

float AIRCharacterNonPlayer::GetAITurnSpeed()
{
	return 3.f;
}

void AIRCharacterNonPlayer::SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void AIRCharacterNonPlayer::AttackByAI()
{
	ProcessAttack();
}

bool AIRCharacterNonPlayer::GetIsHitting()
{
	return bIsHitting;
}

void AIRCharacterNonPlayer::NotifyComboEnd()
{
	Super::NotifyComboEnd();
	OnAttackFinished.ExecuteIfBound();
}
