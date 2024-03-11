// Fill out your copyright notice in the Description page of Project Settings.

#include "IRSaveGame.h"

UIRSaveGame::UIRSaveGame()
{
	BGMVolumeValue = 0.5f;
	SEVolumeValue = 0.5f;
	CurrentLanguage = TEXT("ko");
	MoneyAmount = 0;

	Inventory.Reset();
	Inventory.Add(TEXT("BasicStyle"), true);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(
		TEXT("/Script/Engine.SkeletalMesh'/Game/SCK_Casual01/Models/Premade_Characters/MESH_PC_01.MESH_PC_01'"));
	if (SM.Object)
	{
		EquipedMesh = SM.Object;
	}
}
