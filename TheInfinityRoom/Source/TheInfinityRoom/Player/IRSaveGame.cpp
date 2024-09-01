// Fill out your copyright notice in the Description page of Project Settings.

#include "IRSaveGame.h"
#include "NiagaraFunctionLibrary.h"

UIRSaveGame::UIRSaveGame()
{
	UserName = TEXT("Unknown");

	BGMVolumeValue = 0.5f;
	SEVolumeValue = 0.5f;
	CurrentLanguage = TEXT("ko");
	MoneyAmount = 0;

	Inventory.Reset();
	Inventory.Add(TEXT("BasicStyle"), true);
	Inventory.Add(TEXT("BasicEffect"), true);
	Inventory.Add(TEXT("NoHeadItem"), true);
	Inventory.Add(TEXT("NoBackItem"), true);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(
		TEXT("/Script/Engine.SkeletalMesh'/Game/SCK_Casual01/Models/Premade_Characters/MESH_PC_01.MESH_PC_01'"));
	if (SM.Object)
	{
		EquipedSkin = SM.Object;
	}

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NS(TEXT(
		"/Script/Niagara.NiagaraSystem'/Game/SlashHitVFX/NS/NS_Slash_CurvedSword.NS_Slash_CurvedSword'"));
	if (NS.Object)
	{
		EquipedEffect = NS.Object;
	}

	EquipedItems.Reset();
	EquipedItems.Add(TEXT("Head"), nullptr);
	EquipedItems.Add(TEXT("Back"), nullptr);

	HeadSocketName = TEXT("head_socket1");

	MouseSensitivity = 1.f;
}
