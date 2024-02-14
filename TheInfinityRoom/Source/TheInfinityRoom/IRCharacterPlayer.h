// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IRCharacter.h"
#include "IRCharacterHUDInterface.h"
#include "IRGameOverInterface.h"
#include "IRCharacterPlayer.generated.h"

UENUM()
enum class ECharacterControlType
{
	Default,
	UI
};

UCLASS()
class THEINFINITYROOM_API AIRCharacterPlayer : public AIRCharacter, public IIRCharacterHUDInterface, public IIRGameOverInterface
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	AIRCharacterPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> AttackAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> PauseAction;

protected:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void CancelJump();
	void Attack();
	void Pause();

private:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	TObjectPtr<class UCameraComponent> Camera;

protected:
	virtual void SetDead() override;

protected:
	virtual void SetupHUDWidget(class UIRHUDWidget* InHUDWidget) override;

protected:
	void SetCharacterControl(ECharacterControlType NewCharacterControlType);

	ECharacterControlType CurrentCharacterControlType;

	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = true))
	TMap<ECharacterControlType, class UInputMappingContext*> CharacterControlManager;

public:
	FORCEINLINE ECharacterControlType GetCurrentControlType() { return CurrentCharacterControlType; }

protected:
	virtual void ChangeCharacterControl() override;
};
