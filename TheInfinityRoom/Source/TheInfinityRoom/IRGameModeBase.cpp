// Fill out your copyright notice in the Description page of Project Settings.

#include "IRGameModeBase.h"

AIRGameModeBase::AIRGameModeBase()
{
	static ConstructorHelpers::FClassFinder<APawn> PawnRef(
		TEXT("/Script/CoreUObject.Class'/Script/TheInfinityRoom.IRCharacterPlayer'"));
	if (PawnRef.Succeeded())
	{
		DefaultPawnClass = PawnRef.Class;
	}
}
