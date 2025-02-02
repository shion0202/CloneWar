// Fill out your copyright notice in the Description page of Project Settings.


#include "IRCharacterPlayer.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Stat/IRStatComponent.h"
#include "UI/IRHUDWidget.h"
#include "Interface/IRGameInterface.h"
#include "GameFramework/GameModeBase.h"
#include "Player/IRPlayerController.h"
#include "Game/IRGameModeBase.h"
#include "Player/IRSaveGame.h"
#include "UI/IRWidgetComponent.h"

AIRCharacterPlayer::AIRCharacterPlayer()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 500.f;
	SpringArm->SetRelativeRotation(FRotator(-35.f, 0.f, 0.f));
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMC(
		TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_Default.IMC_Default'"));
	if (IMC.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::Default, IMC.Object);
	}

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMC_UI(
		TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_UI.IMC_UI'"));
	if (IMC_UI.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::UI, IMC_UI.Object);
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_JUMP(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Jump.IA_Jump'"));
	if (IA_JUMP.Object)
	{
		JumpAction = IA_JUMP.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_MOVE(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Move.IA_Move'"));
	if (IA_MOVE.Object)
	{
		MoveAction = IA_MOVE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_LOOK(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Look.IA_Look'"));
	if (IA_LOOK.Object)
	{
		LookAction = IA_LOOK.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_ATTACK(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Attack.IA_Attack'"));
	if (IA_ATTACK.Object)
	{
		AttackAction = IA_ATTACK.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_PAUSE(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Pause.IA_Pause'"));
	if (IA_PAUSE.Object)
	{
		PauseAction = IA_PAUSE.Object;
	}

	CurrentCharacterControlType = ECharacterControlType::Default;
	bIsPlayer = true;

	static ConstructorHelpers::FClassFinder<UUserWidget> UW(TEXT(
		"/Game/UI/WBP_HpBarPlayer.WBP_HpBarPlayer_C"));
	if (UW.Succeeded())
	{
		HpBar->SetWidgetClass(UW.Class);
		HpBar->SetDrawSize(FVector2D(200.f, 25.f));
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	MouseSensitivity = 1.f;
}

void AIRCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	AIRGameModeBase* GameModeBase = Cast<AIRGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameModeBase)
	{
		UIRSaveGame* SaveGameInstance = GameModeBase->GetSaveGameInstance();
		GetMesh()->SetSkeletalMesh(SaveGameInstance->EquipedSkin);
		SetHeadSocket(SaveGameInstance->HeadSocketName);
		EquipHeadItem(*SaveGameInstance->EquipedItems.Find(TEXT("Head")));
		EquipBackItem(*SaveGameInstance->EquipedItems.Find(TEXT("Back")));
		AttackEffect = SaveGameInstance->EquipedEffect;
		bIsReverseEffect = SaveGameInstance->bIsReverseEffect;

		MouseSensitivity = SaveGameInstance->MouseSensitivity;
	}

	SetCharacterControl(CurrentCharacterControlType);
}

void AIRCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent =
		CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AIRCharacterPlayer::CancelJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AIRCharacterPlayer::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AIRCharacterPlayer::Look);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AIRCharacterPlayer::Attack);
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Triggered, this, &AIRCharacterPlayer::Pause);
	}
}

void AIRCharacterPlayer::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void AIRCharacterPlayer::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X * MouseSensitivity);
	AddControllerPitchInput(LookAxisVector.Y * MouseSensitivity);
}

void AIRCharacterPlayer::CancelJump()
{
	GetMesh()->GetAnimInstance()->StopAllMontages(0.f);
	if (bIsHitting)
	{
		HitEnd();
	}

	bPressedJump = true;
	JumpKeyHoldTime = 0.0f;

	CurrentCombo = 0;
	HasNextComboCommand = false;
}

void AIRCharacterPlayer::Attack()
{
	ProcessAttack();
}

void AIRCharacterPlayer::Pause()
{
	AIRPlayerController* PlayerController = Cast<AIRPlayerController>(GetController());
	if (PlayerController)
	{
		PlayerController->OnGamePause();
	}
}

void AIRCharacterPlayer::SetDead()
{
	Super::SetDead();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		DisableInput(PlayerController);

		IIRGameInterface* IRGameMode = Cast<IIRGameInterface>(GetWorld()->GetAuthGameMode());
		if (IRGameMode)
		{
			IRGameMode->OnPlayerDead();
			SetCharacterControl(ECharacterControlType::UI);
		}
	}

	bIsGettingWeapon = false;
	bIsGettingBluePotion = false;
	bIsGettingRedPotion = false;
}

void AIRCharacterPlayer::SetupHUDWidget(UIRHUDWidget* InHUDWidget)
{
	if (InHUDWidget)
	{
		InHUDWidget->UpdateStat(Stat->GetTotalStat());
		Stat->OnStatChanged.AddUObject(InHUDWidget, &UIRHUDWidget::UpdateStat);

		IIRGameInterface* IRGameMode = Cast<IIRGameInterface>(GetWorld()->GetAuthGameMode());
		if (IRGameMode)
		{
			IRGameMode->OnStageLevelChanged.AddUObject(InHUDWidget, &UIRHUDWidget::UpdateStageLevel);
			IRGameMode->OnEnemyCountChanged.AddUObject(InHUDWidget, &UIRHUDWidget::UpdateNumOfEnemies);
			IRGameMode->OnObjectiveChanged.AddUObject(InHUDWidget, &UIRHUDWidget::UpdateObjective);

			// InHUDWidget->UpdateStageLevel(IRGameMode->GetStageLevel());
			InHUDWidget->UpdateStageLevel(true);
		}
	}
}

void AIRCharacterPlayer::SetCharacterControl(ECharacterControlType NewCharacterControlType)
{
	CurrentCharacterControlType = NewCharacterControlType;
	UInputMappingContext* NewCharacterControl = CharacterControlManager[NewCharacterControlType];

	if (APlayerController* PlayerController = CastChecked<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(NewCharacterControl, 0);
			PlayerController->bShowMouseCursor = (CurrentCharacterControlType == ECharacterControlType::Default ? false : true);
		}
	}
}

void AIRCharacterPlayer::ChangeCharacterControl()
{
	if (CurrentCharacterControlType == ECharacterControlType::Default)
	{
		SetCharacterControl(ECharacterControlType::UI);
	}
	else if (CurrentCharacterControlType == ECharacterControlType::UI)
	{
		SetCharacterControl(ECharacterControlType::Default);
	}
}
