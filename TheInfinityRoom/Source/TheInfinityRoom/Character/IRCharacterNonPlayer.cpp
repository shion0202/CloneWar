// Fill out your copyright notice in the Description page of Project Settings.

#include "IRCharacterNonPlayer.h"
#include "AI/IRAIController.h"
#include "Stat/IRStatComponent.h"
#include "UI/IRWidgetComponent.h"

AIRCharacterNonPlayer::AIRCharacterNonPlayer()
{
	AIControllerClass = AIRAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bIsPlayer = false;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(
		TEXT("/Script/Engine.SkeletalMesh'/Game/SCK_Casual01/Models/Premade_Characters/MESH_PC_02.MESH_PC_02'"));
	if (SM.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SM.Object);
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> UW(TEXT(
		"/Game/UI/WBP_HpBar.WBP_HpBar_C"));
	if (UW.Succeeded())
	{
		HpBar->SetWidgetClass(UW.Class);
		HpBar->SetDrawSize(FVector2D(200.f, 25.f));
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
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
