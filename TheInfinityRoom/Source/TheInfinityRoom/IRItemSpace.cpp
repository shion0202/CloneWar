// Fill out your copyright notice in the Description page of Project Settings.


#include "IRItemSpace.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "IRCharacterItemInterface.h"
#include "IRWeaponItemData.h"
#include "Engine/AssetManager.h"

AIRItemSpace::AIRItemSpace()
{
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	RootComponent = Trigger;
	Mesh->SetupAttachment(Trigger);

	Trigger->SetCollisionProfileName("IRCollectable");

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM(
		TEXT("/Script/Engine.StaticMesh'/Game/Assets/Potion_A/potion_A_Potion_G.potion_A_Potion_G'"));
	if (SM.Succeeded())
	{
		Mesh->SetStaticMesh(SM.Object);
	}
	Mesh->SetCollisionProfileName("NoCollision");
}

void AIRItemSpace::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UAssetManager& AssetManager = UAssetManager::Get();
	TArray<FPrimaryAssetId> Assets;
	AssetManager.GetPrimaryAssetIdList(TEXT("IRItemData"), Assets);
	ensure(Assets.Num() > 0);

	int32 RandomIndex = FMath::RandRange(0, Assets.Num() - 1);
	FSoftObjectPtr AssetPtr(AssetManager.GetPrimaryAssetPath(Assets[RandomIndex]));
	if (AssetPtr.IsPending())
	{
		AssetPtr.LoadSynchronous();
	}
	Item = Cast<UIRItemData>(AssetPtr.Get());

	if (Item)
	{
		if (Item->ItemMesh.IsPending())
		{
			Item->ItemMesh.LoadSynchronous();
		}

		Mesh->SetStaticMesh(Item->ItemMesh.Get());
	}
	const float Center = Mesh->GetStaticMesh()->GetBounds().GetBox().GetCenter().Z;
	const float HalfSize = Mesh->GetStaticMesh()->GetBounds().GetBox().GetSize().Z * 0.5f;
	Mesh->SetRelativeLocation(FVector(0.f, 0.f, HalfSize - Center));

	Trigger->SetBoxExtent(Mesh->GetStaticMesh()->GetBounds().GetBox().GetSize());
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AIRItemSpace::OnBeginOverlap);
}

void AIRItemSpace::BeginPlay()
{
	Super::BeginPlay();
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

	Mesh->SetHiddenInGame(true);
	SetActorEnableCollision(false);
}
