#pragma once

#include "CoreMinimal.h"
#include "IRItem.h"
#include "IRSkinItem.generated.h"

USTRUCT(BlueprintType)
struct FIRSkinItem : public FIRItem
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	TObjectPtr<USkeletalMesh> SkeletalMesh;
};