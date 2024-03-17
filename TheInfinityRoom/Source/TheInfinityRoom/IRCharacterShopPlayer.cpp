// Fill out your copyright notice in the Description page of Project Settings.

#include "IRCharacterShopPlayer.h"
#include "IRWidgetComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "IRCameraData.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"

void AIRCharacterShopPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent =
		CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(RotationAction, ETriggerEvent::Triggered, this, &AIRCharacterShopPlayer::Look);
	}
}

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

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMC(
		TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_Shop.IMC_Shop'"));
	if (IMC.Object)
	{
		InputMappingContext = IMC.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_Rotation(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Look.IA_Look'"));
	if (IA_Rotation.Object)
	{
		RotationAction = IA_Rotation.Object;
	}
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

	if (APlayerController* PlayerController = CastChecked<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}

void AIRCharacterShopPlayer::Look(const FInputActionValue& Value)
{
	FVector2D RotationVector = Value.Get<FVector2D>();
	// AddActorLocalRotation(FRotator(0.f, RotationVector.X, 0.f));
}
