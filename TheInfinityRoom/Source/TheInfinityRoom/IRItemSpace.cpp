// Fill out your copyright notice in the Description page of Project Settings.


#include "IRItemSpace.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "IRCharacterItemInterface.h"
#include "IRWeaponItemData.h"

// Sets default values
AIRItemSpace::AIRItemSpace()
{
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));

	RootComponent = Trigger;
	SkeletalMesh->SetupAttachment(Trigger);

	Trigger->SetCollisionProfileName("IRCollectable");
	Trigger->SetBoxExtent(FVector(30.f, 30.f, 30.f));
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AIRItemSpace::OnBeginOverlap);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT(
		"/Script/Engine.SkeletalMesh'/Game/InfinityBladeWeapons/Weapons/Blade/Axes/Blade_AnthraciteAxe/SK_Blade_AnthraciteAxe.SK_Blade_AnthraciteAxe'"));
	if (SM.Succeeded())
	{
		SkeletalMesh->SetSkeletalMesh(SM.Object);
	}
	SkeletalMesh->SetRelativeLocation(FVector(0.f, 0.f, 10.f));
	SkeletalMesh->SetCollisionProfileName("NoCollision");
}

// Called when the game starts or when spawned
void AIRItemSpace::BeginPlay()
{
	Super::BeginPlay();

	// Check item has mesh.
	UIRWeaponItemData* Weapon = Cast<UIRWeaponItemData>(Item);
	if (Weapon)
	{
		SkeletalMesh->SetSkeletalMesh(Weapon->WeaponMesh);
	}
}

void AIRItemSpace::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSwwep, const FHitResult& SweepHitResult)
{
	if (nullptr == Item)
	{
		Destroy();
		return;
	}

	IIRCharacterItemInterface* OverlappingPawn = Cast<IIRCharacterItemInterface>(OtherActor);
	if (OverlappingPawn)
	{
		OverlappingPawn->TakeItem(Item);
	}

	SkeletalMesh->SetHiddenInGame(true);
	SetActorEnableCollision(false);
}
