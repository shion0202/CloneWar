// Fill out your copyright notice in the Description page of Project Settings.


#include "IRAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UIRAnimInstance::UIRAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM(
		TEXT("/Script/Engine.AnimMontage'/Game/Animations/Boris_Skeleton_Montage.Boris_Skeleton_Montage'"));
	if (AM.Succeeded())
	{
		AttackMontage = AM.Object;
	}
}

void UIRAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ACharacter>(GetOwningActor());
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void UIRAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		Speed = Movement->Velocity.Size();
		bIsFalling = Movement->IsFalling();
		VerticalSpeed = Movement->Velocity.Size2D();
	}
}

void UIRAnimInstance::PlayAttackMontage()
{
	Montage_Play(AttackMontage, 1.f);
}

FName UIRAnimInstance::GetAttackMontageName(int32 SectionIndex)
{
	return FName(*FString::Printf(TEXT("Attack%d"), SectionIndex));
}

void UIRAnimInstance::JumpToSection(int32 SectionIndex)
{
	FName Name = GetAttackMontageName(SectionIndex);
	Montage_JumpToSection(Name, AttackMontage);
}

void UIRAnimInstance::AnimNotify_AttackHit()
{
	UE_LOG(LogTemp, Warning, TEXT("AnimNotify_AttackHit"));
}
