// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "SteamManager.h"
#include "IRGameInstance.generated.h"

UCLASS()
class THEINFINITYROOM_API UIRGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UIRGameInstance();

    UPROPERTY(BlueprintReadOnly, Category = Steamworks)
    TObjectPtr<USteamManager> SteamManager;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Steamworks)
    bool IsSteamOverlayActive;

    const bool EnableUSteamManagerFeatures = true;

    UFUNCTION(BlueprintCallable, Category = Steamworks)
    bool InitializeCPPElements();

    void PublicOnSteamOverlayIsON();
    void PublicOnSteamOverlayIsOFF();

    UFUNCTION(BlueprintImplementableEvent, Category = Steamworks)
    void OnSteamOverlayIsActive(bool IsOverlayActive);
};
