// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "IRCharacter.generated.h"

UCLASS()
class THEINFINITYROOM_API AIRCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AIRCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	void ProcessAttack();
	void AttackHitCheck();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

public:
	void ComboBegin();
	void ComboEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
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
};
