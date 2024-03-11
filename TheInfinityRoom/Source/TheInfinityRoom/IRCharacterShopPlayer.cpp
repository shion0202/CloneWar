// Fill out your copyright notice in the Description page of Project Settings.

#include "IRCharacterShopPlayer.h"
#include "IRWidgetComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "IRCameraData.h"

AIRCharacterShopPlayer::AIRCharacterShopPlayer()
{
	static ConstructorHelpers::FObjectFinder<UDataAsset> DA_Title(
		TEXT("/Script/TheInfinityRoom.IRCameraData'/Game/Camera/DA_IRTitleCameraData.DA_IRTitleCameraData'"));
	if (DA_Title.Succeeded())
	{
		TitleCameraData = Cast<UIRCameraData>(DA_Title.Object);
	}

	static ConstructorHelpers::FObjectFinder<UDataAsset> DA_Shop(
		TEXT("/Script/TheInfinityRoom.IRCameraData'/Game/Camera/DA_IRShopCameraData.DA_IRShopCameraData'"));
	if (DA_Shop.Succeeded())
	{
		ShopCameraData = Cast<UIRCameraData>(DA_Shop.Object);
	}

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;

	bIsPlayer = false;
}

void AIRCharacterShopPlayer::SetCameraTransform(bool IsEnterShop)
{
	UIRCameraData* CameraData;

	if (IsEnterShop)
	{
		CameraData = ShopCameraData;
	}
	else
	{
		CameraData = TitleCameraData;
	}

	SpringArm->TargetArmLength = CameraData->CameraLength;
	SpringArm->SetRelativeLocation(CameraData->CameraLocation);
	SpringArm->SetRelativeRotation(CameraData->CameraRotation);
}

void AIRCharacterShopPlayer::BeginPlay()
{
	Super::BeginPlay();

	SetCameraTransform(false);
	HpBar->SetHiddenInGame(true);
}
