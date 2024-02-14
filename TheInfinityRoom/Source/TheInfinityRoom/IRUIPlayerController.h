// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "IRUIPlayerController.generated.h"

UENUM()
enum class EWidgetType
{
	Help,
	License
};

/**
 * 
 */
UCLASS()
class THEINFINITYROOM_API AIRUIPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AIRUIPlayerController();

	void OnDisplayWidget(EWidgetType Type);
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI)
	TObjectPtr<class UUserWidget> UIWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UIRTitleWidget> TitleWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI)
	TObjectPtr<class UIRTitleWidget> TitleWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UUserWidget> HelpWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UUserWidget> LicenseWidgetClass;
};
