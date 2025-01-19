// Fill out your copyright notice in the Description page of Project Settings.

#include "IRCharacterShopPlayer.h"
#include "UI/IRWidgetComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameData/IRCameraData.h"
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
		EnhancedInputComponent->BindAction(RotationAction, ETriggerEvent::Triggered, this, &AIRCharacterShopPlayer::Rotation);
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

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;

	bIsPlayer = false;

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMC(
		TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_Shop.IMC_Shop'"));
	if (IMC.Succeeded())
	{
		InputMappingContext = IMC.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_Rotation(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Rotation.IA_Rotation'"));
	if (IA_Rotation.Succeeded())
	{
		RotationAction = IA_Rotation.Object;
	}

	DefaultRotator = FRotator(0.f, 180.f, 0.f);
	SetActorRotation(DefaultRotator);
}

void AIRCharacterShopPlayer::Rotation(const FInputActionValue& Value)
{
	FVector2D RotationAxisVector = Value.Get<FVector2D>();
	AddActorLocalRotation(FRotator(0.f, RotationAxisVector.X * (-3.f), 0.f));
}

void AIRCharacterShopPlayer::SetDefualtPosition()
{
	// GetWorld()->GetTimerManager().SetTimer(RotationTimerHandle, this, &AIRCharacterShopPlayer::RotateCharacter, 0.001f, true);
	SetActorRotation(DefaultRotator);
}

void AIRCharacterShopPlayer::RotateCharacter()
{
	FRotator CurrentRotation = GetActorRotation();
	float CurrentYaw = CurrentRotation.Yaw;
	
	float DeltaYaw = DefaultRotator.Yaw - CurrentYaw;
	if (FMath::Abs(DeltaYaw) < 0.1f)
	{
		CurrentRotation.Yaw = DefaultRotator.Yaw;
		SetActorRotation(CurrentRotation);
		GetWorld()->GetTimerManager().ClearTimer(RotationTimerHandle);
		return;
	}

	float RotationSpeed = 720.f;
	float Step = FMath::Sign(DeltaYaw) * RotationSpeed * 0.001f;
	if (FMath::Abs(Step) > FMath::Abs(DeltaYaw))
	{
		Step = DeltaYaw;
	}
	CurrentRotation.Yaw += Step;
	SetActorRotation(CurrentRotation);
}

void AIRCharacterShopPlayer::BeginPlay()
{
	Super::BeginPlay();

	SpringArm->TargetArmLength = TitleCameraData->CameraLength;
	SpringArm->SetRelativeLocation(TitleCameraData->CameraLocation);
	SpringArm->SetRelativeRotation(TitleCameraData->CameraRotation);
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
