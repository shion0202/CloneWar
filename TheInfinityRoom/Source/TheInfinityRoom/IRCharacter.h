// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "IRCharacterItemInterface.h"
#include "IRCharacterWidgetInterface.h"
#include "IRCharacterStat.h"
#include "IRCharacter.generated.h"

DECLARE_DELEGATE_OneParam(FOnTakeItemDelegate, class UIRItemData* /* InItemData */)
USTRUCT(BlueprintType)
struct FTakeItemDelegateWrapper
{
	GENERATED_BODY()
	
	FTakeItemDelegateWrapper() {}
	FTakeItemDelegateWrapper(const FOnTakeItemDelegate& InTakeItemDelegate) : TakeItemDelegate(InTakeItemDelegate) {}

	FOnTakeItemDelegate TakeItemDelegate;
};

UCLASS()
class THEINFINITYROOM_API AIRCharacter : public ACharacter, public IIRCharacterItemInterface, public IIRCharacterWidgetInterface
{
	GENERATED_BODY()

public:
	AIRCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	void ProcessAttack();
	void AttackHitCheck();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

public:
	void ComboBegin();
	void ComboEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
	virtual void NotifyComboEnd();
	void SetComboCheckTimer();
	void ComboCheck();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UAnimMontage> AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UIRComboData> ComboData;

	int32 CurrentCombo = 0;
	FTimerHandle ComboTimerHandle;
	bool HasNextComboCommand = false;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UAnimMontage> DeadMontage;

	virtual void SetDead();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UAnimMontage> HitMontage;

	void HitEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UStaticMeshComponent> Weapon;

	UPROPERTY()
	TArray<FTakeItemDelegateWrapper> TakeItemActions;

	FName WeaponSocket = TEXT("hand_rSocket");

	virtual void TakeItem(class UIRItemData* InItemData) override;
	virtual void EquipWeapon(class UIRItemData* InItemData);
	virtual void DrinkPotion(class UIRItemData* InItemData);
	virtual void ReadScroll(class UIRItemData* InItemData);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UIRStatComponent> Stat;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UIRWidgetComponent> HpBar;

	virtual void SetupCharacterWidget(class UIRUserWidget* InUserWidget) override;

	void UpdateStat(const FIRCharacterStat& InTotalStat);

public:
	int32 GetLevel();
	void SetLevel(int32 NewLevel);

protected:
	uint8 bIsPlayer : 1;
	uint8 bIsHitting : 1;
};
