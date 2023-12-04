// Fill out your copyright notice in the Description page of Project Settings.


#include "IRCharacterNonPlayer.h"

AIRCharacterNonPlayer::AIRCharacterNonPlayer()
{

}

void AIRCharacterNonPlayer::SetDead()
{
	Super::SetDead();

	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			Destroy();
		}
	), 2.f, false);
}
