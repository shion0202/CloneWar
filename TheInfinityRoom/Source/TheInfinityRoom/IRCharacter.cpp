// Fill out your copyright notice in the Description page of Project Settings.


#include "IRCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "IRAnimInstance.h"
#include "IRComboData.h"
#include "Engine/DamageEvents.h"

// Sets default values
AIRCharacter::AIRCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationRoll = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("IRCharacter"));

	GetMesh()->SetRelativeLocationAndRotation(
		FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 700.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(
		TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonBoris/Characters/Heroes/Boris/Meshes/Boris.Boris'"));
	if (SM.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SM.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AI(
		TEXT("/Game/Blueprint/ABP_IRAnimInstance.ABP_IRAnimInstance_C"));
	if (AI.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AI.Class);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM(
		TEXT("/Script/Engine.AnimMontage'/Game/Animations/AM_Attack.AM_Attack'"));
	if (AM.Succeeded())
	{
		AttackMontage = AM.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DM(
		TEXT("/Script/Engine.AnimMontage'/Game/Animations/AM_Dead.AM_Dead'"));
	if (DM.Succeeded())
	{
		DeadMontage = DM.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataAsset> CD(
		TEXT("/Script/TheInfinityRoom.IRComboData'/Game/Action/DA_IRComboData.DA_IRComboData'"));
	if (CD.Succeeded())
	{
		ComboData = Cast<UIRComboData>(CD.Object);
	}
}

// Called when the game starts or when spawned
void AIRCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void AIRCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UIRAnimInstance* AnimInstance = Cast<UIRAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->OnAttackHit.AddUObject(this, &AIRCharacter::AttackHitCheck);
	}
}

// Called every frame
void AIRCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AIRCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AIRCharacter::ProcessAttack()
{
	if (GetCharacterMovement()->IsFalling())
	{
		return;
	}

	if (CurrentCombo == 0)
	{
		ComboBegin();
		return;
	}

	if (!ComboTimerHandle.IsValid())
	{
		HasNextComboCommand = false;
	}
	else
	{
		HasNextComboCommand = true;
	}
}

void AIRCharacter::AttackHitCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);

	const float AttackRange = 200.f;
	const float AttackRadius = 50.f;

	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(AttackRadius),
		Params
	);
	if (bResult)
	{
		FDamageEvent DamageEvent;
		HitResult.GetActor()->TakeDamage(10.f, DamageEvent, GetController(), this);
	}

#if ENABLE_DRAW_DEBUG

	FVector Vec = GetActorForwardVector() * AttackRange;
	FVector CapsuleOrigin = GetActorLocation() + Vec * 0.5f;
	float CapsuleHalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat CapsuleRotation = FRotationMatrix::MakeFromZ(Vec).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;

	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius,
		CapsuleRotation, DrawColor, false, 3.0f);

#endif
}

float AIRCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	SetDead();
	return DamageAmount;
}

void AIRCharacter::ComboBegin()
{
	CurrentCombo = 1;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	const float AttackSpeedRate = 1.f;
	GetMesh()->GetAnimInstance()->Montage_Play(AttackMontage, AttackSpeedRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AIRCharacter::ComboEnd);
	GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(EndDelegate, AttackMontage);

	ComboTimerHandle.Invalidate();
	SetComboCheckTimer();
}

void AIRCharacter::ComboEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	CurrentCombo = 0;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void AIRCharacter::SetComboCheckTimer()
{
	const float AttackSpeedRate = 1.f;
	float EffectiveComboTime = (ComboData->EffectiveFrameCount[CurrentCombo - 1] /
		ComboData->FrameRate) / AttackSpeedRate;
	if (EffectiveComboTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this,
			&AIRCharacter::ComboCheck, EffectiveComboTime, false);
	}
}

void AIRCharacter::ComboCheck()
{
	ComboTimerHandle.Invalidate();
	if (HasNextComboCommand)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboData->MaxComboCount);
		FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboData->MontageSectionName,
			CurrentCombo);
		AnimInstance->Montage_JumpToSection(NextSection, AttackMontage);
		SetComboCheckTimer();
		HasNextComboCommand = false;
	}
}

void AIRCharacter::SetDead()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	SetActorEnableCollision(false);

	UIRAnimInstance* AnimInstance = Cast<UIRAnimInstance>(GetMesh()->GetAnimInstance());
	AnimInstance->StopAllMontages(0.f);
	AnimInstance->Montage_Play(DeadMontage);
}
