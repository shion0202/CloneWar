// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IRCharacter.h"
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

	void SetCameraTransform(bool IsEnterShop);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	TObjectPtr<class UCameraComponent> Camera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UIRCameraData> TitleCameraData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UIRCameraData> ShopCameraData;
};
