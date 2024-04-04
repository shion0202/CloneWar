// Fill out your copyright notice in the Description page of Project Settings.

#include "SteamManager.h"
#include "IRGameInstance.h"
#include "Async/Async.h"

USteamManager::USteamManager()
{
	UE_LOG(LogTemp, Log, TEXT("USteamManager Constructor Called"));
}

void USteamManager::InitializeSteamManager()
{
	UE_LOG(LogTemp, Log, TEXT("Initializing USteamManager"));
	//OnSteamOverlayActiveCallback.Register(this, &USteamManager::OnSteamOverlayActive);
	UE_LOG(LogTemp, Log, TEXT("OnSteamOverlayActiveCallback.Register called"));
}

void USteamManager::AssignGameInstance(UIRGameInstance* NewGameInstance)
{
    IRGameInstance = NewGameInstance;
	UE_LOG(LogTemp, Log, TEXT("New Game Instance Assigned to USteamManager"));
}

//void USteamManager::OnSteamOverlayActive(GameOverlayActivated_t* pCallbackData)
//{
//    UE_LOG(LogTemp, Log, TEXT("Intercepted the Steam Overlay callback"));
//    const bool IsCurrentOverlayActive = ((pCallbackData->m_bActive) != (0));;
//    UE_LOG(LogTemp, Log, TEXT("isCurrentOverlayActive = %d"), IsCurrentOverlayActive);
//    IRGameInstance;
//
//    if (IsCurrentOverlayActive)
//    {
//        AsyncTask(ENamedThreads::GameThread, [&]() {
//            UE_LOG(LogTemp, Log, TEXT("Running inside AsyncTask() TRUE"));
//            IRGameInstance->PublicOnSteamOverlayIsON();
//            UE_LOG(LogTemp, Log, TEXT("Exiting AsyncTask()"));
//            });
//    }
//    else {
//        AsyncTask(ENamedThreads::GameThread, [&]() {
//            UE_LOG(LogTemp, Log, TEXT("Running inside AsyncTask() FALSE"));
//            IRGameInstance->PublicOnSteamOverlayIsOFF();
//            UE_LOG(LogTemp, Log, TEXT("Exiting AsyncTask()"));
//            });
//    }
//    UE_LOG(LogTemp, Log, TEXT("OnSteamOverlayActive() at SteamManager Finished"));
//}
