// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Interface/IRCharacterItemInterface.h"
#include "Interface/IRCharacterWidgetInterface.h"
#include "GameData/IRCharacterStat.h"
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

	void HitEnd();

public:
	void EquipHeadItem(class UStaticMesh* ItemMesh);
	void EquipBackItem(class UStaticMesh* ItemMesh);

	void SetHeadSocket(FName SocketName);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UStaticMeshComponent> Weapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UStaticMeshComponent> HeadItem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UStaticMeshComponent> BackItem;

	UPROPERTY()
	TArray<FTakeItemDelegateWrapper> TakeItemActions;

	FName WeaponSocket = TEXT("hand_rSocket");
	FName HeadSocket = TEXT("");
	FName BackSocket = TEXT("spine_socket");

	virtual void TakeItem(class UIRItemData* InItemData) override;
	virtual void EquipWeapon(class UIRItemData* InItemData);
	virtual void DrinkPotion(class UIRItemData* InItemData);
	virtual void ReadScroll(class UIRItemData* InItemData);
	virtual void GetNothing(class UIRItemData* InItemData);

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

public:
	void PlayEffectForPreview(class UNiagaraSystem* InEffect);

protected:
	void PlayAttackEffect();

protected:
	UPROPERTY(EditDefaultsOnly, Category = Effect)
	TObjectPtr<class UNiagaraSystem> AttackEffect;

	UPROPERTY(EditDefaultsOnly, Category = Effect)
	TObjectPtr<class UNiagaraComponent> SpawnedEffect;

	UPROPERTY(EditDefaultsOnly, Category = Sound)
	TObjectPtr<class USoundWave> GetItemSoundWave;
};
