#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "IRItem.generated.h"

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
};