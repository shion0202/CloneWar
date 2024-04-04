#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "IRCredit.generated.h"

USTRUCT(BlueprintType)
struct FIRCredit : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	FIRCredit() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	FText Author;
};