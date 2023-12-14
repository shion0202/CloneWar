// Fill out your copyright notice in the Description page of Project Settings.


#include "IRCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "IRAnimInstance.h"
#include "IRComboData.h"
#include "Engine/DamageEvents.h"
#include "IRWeaponItemData.h"
#include "IRStatComponent.h"
#include "IRWidgetComponent.h"
#include "IRHpBarWidget.h"

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

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), WeaponSocket);
	Weapon->SetCollisionProfileName("NoCollision");

	TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(
		this, &AIRCharacter::EquipWeapon)));
	TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(
		this, &AIRCharacter::DrinkPotion)));
	TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(
		this, &AIRCharacter::ReadScroll)));

	Stat = CreateDefaultSubobject<UIRStatComponent>(TEXT("Stat"));

	HpBar = CreateDefaultSubobject<UIRWidgetComponent>(TEXT("HpBar"));
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetWidgetSpace(EWidgetSpace::Screen);
	HpBar->SetRelativeLocation(FVector(0.f, 0.f, 250.f));
	static ConstructorHelpers::FClassFinder<UUserWidget> UW(TEXT(
		"/Game/UI/WBP_HpBar.WBP_HpBar_C"));
	if (UW.Succeeded())
	{
		HpBar->SetWidgetClass(UW.Class);
		HpBar->SetDrawSize(FVector2D(200.f, 20.f));
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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

	HpBar->InitWidget();
	Stat->OnHpZero.AddUObject(this, &AIRCharacter::SetDead);
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

	const float AttackRange = 220.f;
	const float AttackRadius = 120.f;
	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = GetActorLocation() + GetActorForwardVector() * AttackRange;

	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		Start,
		End,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(AttackRadius),
		Params
	);
	if (bResult)
	{
		FDamageEvent DamageEvent;
		HitResult.GetActor()->TakeDamage(Stat->GetAttack(), DamageEvent, GetController(), this);
	}

#if ENABLE_DRAW_DEBUG

	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	float CapsuleHalfHeight = AttackRange * 0.5f;
	FQuat CapsuleRotation = FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;

	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius,
		CapsuleRotation, DrawColor, false, 3.0f);

#endif
}

float AIRCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	Stat->ApplyDamage(DamageAmount);
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

void AIRCharacter::TakeItem(UIRItemData* InItemData)
{
	if (InItemData)
	{
		TakeItemActions[(uint8)InItemData->Type].TakeItemDelegate.ExecuteIfBound(InItemData);
	}
}

void AIRCharacter::EquipWeapon(UIRItemData* InItemData)
{
	UIRWeaponItemData* NewWeapon = Cast<UIRWeaponItemData>(InItemData);

	if (GetMesh()->DoesSocketExist(WeaponSocket) && NewWeapon)
	{
		Weapon->SetSkeletalMesh(NewWeapon->WeaponMesh);
	}
}

void AIRCharacter::DrinkPotion(UIRItemData* InItemData)
{
	UE_LOG(LogTemp, Warning, TEXT("Drink Potion"));
}

void AIRCharacter::ReadScroll(UIRItemData* InItemData)
{
	UE_LOG(LogTemp, Warning, TEXT("Read Scroll"));
}

void AIRCharacter::SetupCharacterWidget(UIRUserWidget* InUserWidget)
{
	UIRHpBarWidget* HpBarWidget = Cast<UIRHpBarWidget>(InUserWidget);
	if (HpBarWidget)
	{
		HpBarWidget->UpdateHp(1.f);
		Stat->OnHpChanged.AddUObject(HpBarWidget, &UIRHpBarWidget::UpdateHp);
	}
}
