// Fill out your copyright notice in the Description page of Project Settings.


#include "IRItemSpace.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Interface/IRCharacterItemInterface.h"
#include "IRWeaponItemData.h"
#include "Engine/AssetManager.h"
#include "UI/IRWidgetComponent.h"
#include "UI/IRItemInfoWidget.h"

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
	Mesh->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));

	bIsNoneItem = false;

	InformationWidget = CreateDefaultSubobject<UIRWidgetComponent>(TEXT("ItemInfo"));
	InformationWidget->SetupAttachment(Mesh);
	InformationWidget->SetWidgetSpace(EWidgetSpace::World);

	static ConstructorHelpers::FClassFinder<UUserWidget> UW(TEXT(
		"/Game/UI/WBP_ItemInformation.WBP_ItemInformation_C"));
	if (UW.Succeeded())
	{
		InformationWidget->SetWidgetClass(UW.Class);
		InformationWidget->SetDrawSize(FVector2D(200.f, 100.f));
		InformationWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AIRItemSpace::SetupSpaceWidget(UIRUserWidget* InUserWidget)
{
	UIRItemInfoWidget* ItemInfoWidget = Cast<UIRItemInfoWidget>(InUserWidget);
	if (ItemInfoWidget)
	{
		ItemInfoWidget->UpdateItemInformation(Item->ItemName, Item->ItemDescription);
	}
}

void AIRItemSpace::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (bIsNoneItem)
	{
		LoadNoneItem();
	}
	else
	{
		LoadItems();
	}

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
	Mesh->SetRelativeLocation(FVector(0.f, 0.f, HalfSize - Center + 20.f));

	Trigger->SetBoxExtent(Mesh->GetStaticMesh()->GetBounds().GetBox().GetSize());
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AIRItemSpace::OnBeginOverlap);

	if (bIsNoneItem)
	{
		InformationWidget->SetRelativeLocation(FVector(0.f, 0.f, 50.f));
	}
	else
	{
		InformationWidget->SetRelativeLocation(FVector(0.f, 0.f, 0.f + Center + 150.f));
	}

	InformationWidget->InitWidget();
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
	InformationWidget->SetHiddenInGame(true);
	SetActorEnableCollision(false);
}

void AIRItemSpace::LoadItems()
{
	UAssetManager& AssetManager = UAssetManager::Get();
	TArray<TArray<FPrimaryAssetId>> Assets;
	Assets.SetNum(Typenames.Num());

	for (int i = 0; i < Assets.Num(); ++i)
	{
		AssetManager.GetPrimaryAssetIdList(Typenames[i], Assets[i]);
		ensure(Assets[i].Num() > 0);
	}

	int32 TypeIndex = FMath::RandRange(0, 99);
	int RandomIndex = 0;
	if (TypeIndex < ItemPercent[0])
	{
		TypeIndex = 0;
		RandomIndex = FMath::RandRange(0, Assets[TypeIndex].Num() - 1);
	}
	else if (TypeIndex < ItemPercent[0] + ItemPercent[1])
	{
		TypeIndex = 1;
		RandomIndex = FMath::RandRange(0, Assets[TypeIndex].Num() - 1);
	}
	else
	{
		TypeIndex = 2;
		RandomIndex = FMath::RandRange(0, Assets[TypeIndex].Num() - 1);
	}

	FSoftObjectPtr AssetPtr(AssetManager.GetPrimaryAssetPath(Assets[TypeIndex][RandomIndex]));
	if (AssetPtr.IsPending())
	{
		AssetPtr.LoadSynchronous();
	}
	Item = Cast<UIRItemData>(AssetPtr.Get());
}

void AIRItemSpace::LoadNoneItem()
{
	UAssetManager& AssetManager = UAssetManager::Get();
	TArray<FPrimaryAssetId> Assets;
	AssetManager.GetPrimaryAssetIdList(TEXT("IRNoneItemData"), Assets);
	ensure(Assets.Num() > 0);

	FSoftObjectPtr AssetPtr(AssetManager.GetPrimaryAssetPath(Assets[0]));
	if (AssetPtr.IsPending())
	{
		AssetPtr.LoadSynchronous();
	}
	Item = Cast<UIRItemData>(AssetPtr.Get());
}
