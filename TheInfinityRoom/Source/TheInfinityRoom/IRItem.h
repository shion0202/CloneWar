#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "NiagaraFunctionLibrary.h"
#include "IRItem.generated.h"

UENUM()
enum class EShopItemType
{
	Skin,
	Head,
	Back,
	Effect
};

USTRUCT(BlueprintType)
struct FIRItem : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	int32 Cost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	TObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	EShopItemType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	TObjectPtr<USkeletalMesh> SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	TObjectPtr<UStaticMesh> StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	TObjectPtr<UNiagaraSystem> Effect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	FName HeadSocketName;
};