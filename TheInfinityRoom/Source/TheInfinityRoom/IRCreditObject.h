// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "IRCredit.h"
#include "IRCreditObject.generated.h"

/**
 * 
 */
UCLASS()
class THEINFINITYROOM_API UIRCreditObject : public UObject
{
	GENERATED_BODY()
	
public:
	UIRCreditObject();

	FORCEINLINE FIRCredit GetCreditData() { return CreditData; }
	FORCEINLINE void SetCreditData(FIRCredit InCredit) { CreditData = InCredit; }

protected:
	FIRCredit CreditData;
};
