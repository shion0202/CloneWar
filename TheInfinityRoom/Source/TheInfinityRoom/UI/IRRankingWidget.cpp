// Fill out your copyright notice in the Description page of Project Settings.

#include "IRRankingWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Game/IRGameInstance.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Player/IRUIPlayerController.h"

void UIRRankingWidget::NativeConstruct()
{
	if (nullptr != BTN_RankStage)
	{
		BTN_RankStage->OnClicked.AddDynamic(this, &UIRRankingWidget::OnRankStageClick);
	}

	if (nullptr != BTN_RankKill)
	{
		BTN_RankKill->OnClicked.AddDynamic(this, &UIRRankingWidget::OnRankKillClick);
	}

	if (nullptr != BTN_Close)
	{
		BTN_Close->OnClicked.AddDynamic(this, &UIRRankingWidget::OnCloseClick);
	}

	UIRGameInstance* GI = Cast<UIRGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI)
	{
		Stats = GI->GetStatScore();
		if (!Stats.IsEmpty())
		{
			SetStats();
		}
	}
}

void UIRRankingWidget::OnRankStageClick()
{
	RemoveFromParent();

	AIRUIPlayerController* PlayerController = Cast<AIRUIPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		PlayerController->OnDisplayWidget(EWidgetType::RankStage);
	}
}

void UIRRankingWidget::OnRankKillClick()
{
	RemoveFromParent();

	AIRUIPlayerController* PlayerController = Cast<AIRUIPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		PlayerController->OnDisplayWidget(EWidgetType::RankKill);
	}
}

void UIRRankingWidget::OnCloseClick()
{
	AIRUIPlayerController* PlayerController = Cast<AIRUIPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		PlayerController->EnableButtons();
	}

	RemoveFromParent();
}

int32 UIRRankingWidget::GetStat(FString Key)
{
	if (int32* FoundValue = Stats.Find(Key))
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1, 3.f, FColor::Orange, FString::Printf(TEXT("%s Score: %d"), *Key, *FoundValue)
			);
		}

		return *FoundValue;
	}

	UE_LOG(LogTemp, Warning, TEXT("Can't find the key."));
	return -1;
}

void UIRRankingWidget::SetStats()
{
	TXT_NewGameCount->SetText(FText::FromString(FString::FromInt(GetStat("NewGameAmount_NewGameAmount"))));
	TXT_HighestStageLevel->SetText(FText::FromString(FString::FromInt(GetStat("HighestStageLevel_HighestStageLevel"))));
	TXT_DestroyEnemyCount->SetText(FText::FromString(FString::FromInt(GetStat("KillEnemyAmount_KillEnemyAmount"))));
	TXT_GetMoneyCount->SetText(FText::FromString(FString::FromInt(GetStat("GetMoneyAmount_GetMoneyAmount"))));
	TXT_UseMoneyCount->SetText(FText::FromString(FString::FromInt(GetStat("UseMoneyAmount_UseMoneyAmount"))));
}
