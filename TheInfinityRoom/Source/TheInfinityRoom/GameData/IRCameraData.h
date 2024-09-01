// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "IRCameraData.generated.h"

/**
 * 
 */
UCLASS()
class THEINFINITYROOM_API UIRCameraData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UIRCameraData();

	UPROPERTY(EditAnywhere, Category = Camera)
	float CameraLength;

	UPROPERTY(EditAnywhere, Category = Camera)
	FVector CameraLocation;

	UPROPERTY(EditAnywhere, Category = Camera)
	FRotator CameraRotation;
};
