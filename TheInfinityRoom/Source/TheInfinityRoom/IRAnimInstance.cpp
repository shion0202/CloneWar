// Fill out your copyright notice in the Description page of Project Settings.


#include "IRAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UIRAnimInstance::UIRAnimInstance()
{
	MovingThreshould = 3.f;
	HorizontalSpeed = 0.f;
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
		VerticalSpeed = Movement->Velocity.Size2D();
		bIsIdle = VerticalSpeed < MovingThreshould;
		bIsFalling = Movement->IsFalling();
	}
}

void UIRAnimInstance::AnimNotify_AttackHit()
{
	OnAttackHit.Broadcast();
}

void UIRAnimInstance::AnimNotify_AttackEffect()
{
	OnAttackEffect.Broadcast();
}

void UIRAnimInstance::AnimNotify_HitEnd()
{
	OnHitEnd.Broadcast();
}
