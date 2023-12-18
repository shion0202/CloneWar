// Fill out your copyright notice in the Description page of Project Settings.


#include "IRGameModeBase.h"

AIRGameModeBase::AIRGameModeBase()
{
	// DefaultPawnClass = AMyCharacter::StaticClass();
	static ConstructorHelpers::FClassFinder<APawn> AP(
		TEXT("/Script/CoreUObject.Class'/Script/TheInfinityRoom.IRCharacterPlayer'"));
	if (AP.Succeeded())
	{
		DefaultPawnClass = AP.Class;
	}
}
