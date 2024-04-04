// Fill out your copyright notice in the Description page of Project Settings.

#include "IRCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "IRAnimInstance.h"
#include "IRComboData.h"
#include "Engine/DamageEvents.h"
#include "IRWeaponItemData.h"
#include "IRPotionItemData.h"
#include "IRScrollItemData.h"
#include "IRStatComponent.h"
#include "IRWidgetComponent.h"
#include "IRHpBarWidget.h"
#include "Kismet/GameplayStatics.h"
#include "IRGameModeBase.h"
#include "IRSaveGame.h"
#include "NiagaraFunctionLibrary.h"

AIRCharacter::AIRCharacter()
{
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
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->AirControl = 0.35f;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(
		TEXT("/Script/Engine.SkeletalMesh'/Game/SCK_Casual01/Models/Premade_Characters/MESH_PC_01.MESH_PC_01'"));
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

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_ATTACK(
		TEXT("/Script/Engine.AnimMontage'/Game/Animations/AM_Attack.AM_Attack'"));
	if (AM_ATTACK.Succeeded())
	{
		AttackMontage = AM_ATTACK.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_DEAD(
		TEXT("/Script/Engine.AnimMontage'/Game/Animations/AM_Dead.AM_Dead'"));
	if (AM_DEAD.Succeeded())
	{
		DeadMontage = AM_DEAD.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_HIT(
		TEXT("/Script/Engine.AnimMontage'/Game/Animations/AM_Hit.AM_Hit'"));
	if (AM_HIT.Succeeded())
	{
		HitMontage = AM_HIT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataAsset> DA(
		TEXT("/Script/TheInfinityRoom.IRComboData'/Game/Action/DA_IRComboData.DA_IRComboData'"));
	if (DA.Succeeded())
	{
		ComboData = Cast<UIRComboData>(DA.Object);
	}

	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WEAPON"));
	Weapon->SetupAttachment(GetMesh(), WeaponSocket);
	Weapon->SetCollisionProfileName("NoCollision");

	HeadItem = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HEAD"));
	HeadItem->SetCollisionProfileName("NoCollision");

	BackItem = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BACK"));
	BackItem->SetupAttachment(GetMesh(), BackSocket);
	BackItem->SetCollisionProfileName("NoCollision");


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
	HpBar->SetRelativeLocation(FVector(0.f, 0.f, 220.f));
	static ConstructorHelpers::FClassFinder<UUserWidget> UW(TEXT(
		"/Game/UI/WBP_HpBar.WBP_HpBar_C"));
	if (UW.Succeeded())
	{
		HpBar->SetWidgetClass(UW.Class);
		HpBar->SetDrawSize(FVector2D(200.f, 25.f));
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	bIsPlayer = false;
	bIsHitting = false;

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> AttackEffectRef(TEXT(
		"/Script/Niagara.NiagaraSystem'/Game/SlashHitVFX/NS/NS_Slash_CurvedSword.NS_Slash_CurvedSword'"));
	if (AttackEffectRef.Object)
	{
		AttackEffect = AttackEffectRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundWave> GetItemSoundWaveRef(TEXT(
		"/Script/Engine.SoundWave'/Game/Sounds/Sfx_Item.Sfx_Item'"));
	if (GetItemSoundWaveRef.Object)
	{
		GetItemSoundWave = GetItemSoundWaveRef.Object;
	}
}

void AIRCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = Stat->GetTotalStat().MovementSpeed;
	GetCharacterMovement()->JumpZVelocity = Stat->GetTotalStat().JumpVelocity;

	AIRGameModeBase* GameModeBase = Cast<AIRGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameModeBase)
	{
		UIRSaveGame* SaveGameInstance = GameModeBase->GetSaveGameInstance();
		SetHeadSocket(SaveGameInstance->HeadSocketName);
	}
}

void AIRCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UIRAnimInstance* AnimInstance = Cast<UIRAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->OnAttackHit.AddUObject(this, &AIRCharacter::AttackHitCheck);
		AnimInstance->OnAttackEffect.AddUObject(this, &AIRCharacter::PlayAttackEffect);
		AnimInstance->OnHitEnd.AddUObject(this, &AIRCharacter::HitEnd);
	}

	HpBar->InitWidget();
	Stat->OnHpZero.AddUObject(this, &AIRCharacter::SetDead);
	Stat->OnStatChanged.AddUObject(this, &AIRCharacter::UpdateStat);
}

void AIRCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AIRCharacter::ProcessAttack()
{
	if (GetCharacterMovement()->IsFalling() || bIsHitting)
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
	TArray<FHitResult> HitResults;
	FCollisionQueryParams Params(NAME_None, false, this);

	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = GetActorLocation() + GetActorForwardVector() * Stat->GetTotalStat().AttackRange;

	bool bResult = GetWorld()->SweepMultiByChannel(
		HitResults,
		Start,
		End,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(Stat->GetTotalStat().AttackRadius),
		Params
	);
	if (bResult)
	{
		FDamageEvent DamageEvent;

		for (auto const& HitResult : HitResults)
		{
			AIRCharacter* Target = Cast<AIRCharacter>(HitResult.GetActor());
			if (Target && bIsPlayer != Target->bIsPlayer)
			{
				HitResult.GetActor()->TakeDamage(Stat->GetTotalStat().AttackDamage, DamageEvent, GetController(), this);
			}
		}
	}

#if ENABLE_DRAW_DEBUG

	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	float CapsuleHalfHeight = Stat->GetTotalStat().AttackRange * 0.5f;
	FQuat CapsuleRotation = FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;

	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, Stat->GetTotalStat().AttackRadius,
		CapsuleRotation, DrawColor, false, 3.0f);

#endif
}

float AIRCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	bIsHitting = true;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	UIRAnimInstance* AnimInstance = Cast<UIRAnimInstance>(GetMesh()->GetAnimInstance());
	AnimInstance->StopAllMontages(0.f);
	AnimInstance->Montage_Play(HitMontage);
	
	Stat->ApplyDamage(DamageAmount);
	return DamageAmount;
}

void AIRCharacter::ComboBegin()
{
	CurrentCombo = 1;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	GetMesh()->GetAnimInstance()->Montage_Play(AttackMontage, Stat->GetTotalStat().AttackSpeed);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AIRCharacter::ComboEnd);
	GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(EndDelegate, AttackMontage);

	ComboTimerHandle.Invalidate();
	SetComboCheckTimer();
}

void AIRCharacter::ComboEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	CurrentCombo = 0;
	
	if (GetWorld()->GetTimerManager().IsTimerActive(ComboTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(ComboTimerHandle);
	}
	
	NotifyComboEnd();

	if (!bIsHitting)
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}
}

void AIRCharacter::NotifyComboEnd()
{
	
}

void AIRCharacter::SetComboCheckTimer()
{
	float EffectiveComboTime = (ComboData->EffectiveFrameCount[CurrentCombo - 1] /
		ComboData->FrameRate) / Stat->GetTotalStat().AttackSpeed;
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
	HpBar->SetHiddenInGame(true);
}

void AIRCharacter::HitEnd()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	bIsHitting = false;
}

void AIRCharacter::EquipHeadItem(UStaticMesh* ItemMesh)
{
	if (GetMesh()->DoesSocketExist(HeadSocket))
	{
		HeadItem->SetStaticMesh(ItemMesh);
	}
}

void AIRCharacter::EquipBackItem(UStaticMesh* ItemMesh)
{
	if (GetMesh()->DoesSocketExist(BackSocket))
	{
		BackItem->SetStaticMesh(ItemMesh);
	}
}

void AIRCharacter::SetHeadSocket(FName SocketName)
{
	HeadSocket = SocketName;
	FAttachmentTransformRules Rule(EAttachmentRule::SnapToTarget, false);
	HeadItem->AttachToComponent(GetMesh(), Rule, HeadSocket);
}

void AIRCharacter::TakeItem(UIRItemData* InItemData)
{
	UGameplayStatics::PlaySound2D(this, GetItemSoundWave);

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
		if (NewWeapon->ItemMesh.IsPending())
		{
			NewWeapon->ItemMesh.LoadSynchronous();
		}

		Weapon->SetStaticMesh(NewWeapon->ItemMesh.Get());
		Stat->SetWeaponStat(NewWeapon->GetWeaponStat());
	}
}

void AIRCharacter::DrinkPotion(UIRItemData* InItemData)
{
	UIRPotionItemData* InPotion = Cast<UIRPotionItemData>(InItemData);
	if (InPotion)
	{
		Stat->HealHp(InPotion->GetHealAmount());
	}
}

void AIRCharacter::ReadScroll(UIRItemData* InItemData)
{
	UIRScrollItemData* InScroll = Cast<UIRScrollItemData>(InItemData);
	if (InScroll)
	{
		Stat->AddScrollStat(InScroll->GetScrollStat());
	}
}

void AIRCharacter::SetupCharacterWidget(UIRUserWidget* InUserWidget)
{
	UIRHpBarWidget* HpBarWidget = Cast<UIRHpBarWidget>(InUserWidget);
	if (HpBarWidget)
	{
		HpBarWidget->UpdateHp(Stat->GetTotalStat().MaxHp, Stat->GetCurrentHp());
		Stat->OnHpChanged.AddUObject(HpBarWidget, &UIRHpBarWidget::UpdateHp);
	}
}

void AIRCharacter::UpdateStat(const FIRCharacterStat& InTotalStat)
{
	GetCharacterMovement()->MaxWalkSpeed = InTotalStat.MovementSpeed;
	GetCharacterMovement()->JumpZVelocity = InTotalStat.JumpVelocity;
}

int32 AIRCharacter::GetLevel()
{
	return Stat->GetLevel();
}

void AIRCharacter::SetLevel(int32 NewLevel)
{
	Stat->SetLevel(NewLevel);
}

void AIRCharacter::PlayEffectForPreview(UNiagaraSystem* InEffect)
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), InEffect, GetActorLocation() + FVector(0.f, 0.f, 20.f),
		FRotator(0.f, 90.f, 45.f));
}

void AIRCharacter::PlayAttackEffect()
{
	FVector EffectLocation = GetActorLocation() + FVector::UpVector * ComboData->EffectLocation[CurrentCombo - 1]
		+ GetActorForwardVector() * ComboData->EffectLocation[CurrentCombo - 1];
	FRotator EffectRotation = GetActorRotation() + ComboData->EffectRotation[CurrentCombo - 1];
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), AttackEffect, EffectLocation, EffectRotation);
}
