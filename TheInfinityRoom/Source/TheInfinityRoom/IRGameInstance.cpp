// Fill out your copyright notice in the Description page of Project Settings.

#include "IRGameInstance.h"

UIRGameInstance::UIRGameInstance()
{

}

bool UIRGameInstance::InitializeCPPElements()
{
    if (EnableUSteamManagerFeatures)
    {
        UE_LOG(LogTemp, Log, TEXT("CPP InitializeCPPElements() through USteamManager"));
        SteamManager = NewObject<USteamManager>(this);
        UE_LOG(LogTemp, Log, TEXT("New Object USteamManager created!"));
        SteamManager->InitializeSteamManager();
        UE_LOG(LogTemp, Log, TEXT("SteamManager Initialization Finished"));
        SteamManager->AssignGameInstance(this);
        UE_LOG(LogTemp, Log, TEXT("CPP InitializeCPPElements() SUCCESS!"));
        return true;
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Custom Steamworks Features NOT Initialized."));
        UE_LOG(LogTemp, Log, TEXT("Change EnableUSteamManagerFeatures to True."));
    }
    return false;
}

void UIRGameInstance::PublicOnSteamOverlayIsON()
{
    UE_LOG(LogTemp, Log, TEXT("PublicOnSteamOverlayIsON Called. Setting isSteamOverlayActive"));
    IsSteamOverlayActive = true;
    //this->OnSteamOverlayIsActive(true);
    UE_LOG(LogTemp, Log, TEXT("OnSteamOverlayIsActive Has been triggered"));
}

void UIRGameInstance::PublicOnSteamOverlayIsOFF()
{
    UE_LOG(LogTemp, Log, TEXT("PublicOnSteamOverlayIsON Called. Setting isSteamOverlayActive"));
    IsSteamOverlayActive = false;
    //this->OnSteamOverlayIsActive(false);
    UE_LOG(LogTemp, Log, TEXT("OnSteamOverlayIsActive Has been triggered"));
}
