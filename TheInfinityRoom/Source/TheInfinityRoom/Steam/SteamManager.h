// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#pragma push_macro("ARRAY_COUNT")
#undef ARRAY_COUNT
THIRD_PARTY_INCLUDES_START
#include "ThirdParty/Steamworks/Steamv159/sdk/public/steam/steam_api.h"
THIRD_PARTY_INCLUDES_END
#pragma pop_macro("ARRAY_COUNT")

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SteamManager.generated.h"

/**
 * 
 */
UCLASS()
class THEINFINITYROOM_API USteamManager : public UObject
{
	GENERATED_BODY()
	
private:
    //STEAM_CALLBACK_MANUAL(USteamManager, OnSteamOverlayActive, GameOverlayActivated_t, OnSteamOverlayActiveCallback);

public:
    USteamManager();

    void InitializeSteamManager();
    void AssignGameInstance(class UIRGameInstance* NewGameInstance);

    UPROPERTY(BlueprintReadOnly, Category = GameInstance)
    TObjectPtr<class UIRGameInstance> IRGameInstance;
};
