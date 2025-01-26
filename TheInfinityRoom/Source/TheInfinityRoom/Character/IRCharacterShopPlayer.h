// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IRCharacter.h"
#include "InputActionValue.h"
#include "IRCharacterShopPlayer.generated.h"

/**
 * 
 */
UCLASS()
class THEINFINITYROOM_API AIRCharacterShopPlayer : public AIRCharacter
{
	GENERATED_BODY()

public:
	AIRCharacterShopPlayer();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Rotation(const FInputActionValue& Value);
	void SetDefualtPosition();
	void RotateCharacter();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	TObjectPtr<class UCameraComponent> Camera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UIRCameraData> TitleCameraData;
	
private:
	UPROPERTY(EditAnywhere, Category = Input, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UInputAction> RotationAction;

	UPROPERTY(EditAnywhere, Category = Input, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UInputMappingContext> InputMappingContext;

	FTimerHandle RotationTimerHandle;
	FRotator DefaultRotator;
};
