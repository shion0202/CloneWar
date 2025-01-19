// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/IRCameraActor.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameData/IRCameraData.h"

// Sets default values
AIRCameraActor::AIRCameraActor()
{
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
}

// Called when the game starts or when spawned
void AIRCameraActor::BeginPlay()
{
	Super::BeginPlay();

	SpringArm->TargetArmLength = ShopCameraData->CameraLength;
	SpringArm->SetRelativeLocation(ShopCameraData->CameraLocation);
	SpringArm->SetRelativeRotation(ShopCameraData->CameraRotation);
}
