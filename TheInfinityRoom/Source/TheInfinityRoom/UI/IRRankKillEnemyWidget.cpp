// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/IRRankKillEnemyWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Player/IRUIPlayerController.h"

void UIRRankKillEnemyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (nullptr != BTN_Statistics)
	{
		BTN_Statistics->OnClicked.AddDynamic(this, &UIRRankKillEnemyWidget::OnStatisticsClick);
	}

	if (nullptr != BTN_RankStage)
	{
		BTN_RankStage->OnClicked.AddDynamic(this, &UIRRankKillEnemyWidget::OnRankStageClick);
	}

	if (nullptr != BTN_Close)
	{
		BTN_Close->OnClicked.AddDynamic(this, &UIRRankKillEnemyWidget::OnCloseClick);
	}

	UIRGameInstance* GI = Cast<UIRGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI)
	{
		TMap<int32, FLeaderboardRow> LeaderboardMap = GI->GetLeaderboards(false);
		for (auto Pair : LeaderboardMap)
		{
			LeaderboardsList.Add(Pair.Value);
		}

		TMap<int32, FLeaderboardRow> MyLeaderboardMap = GI->GetMyLeaderboards();
		MyLeaderboard = *MyLeaderboardMap.Find(1);
	}

	for (int i = 0; i < LeaderboardsList.Num(); ++i)
	{
		const FName RankTextBlockName = *FString::Printf(TEXT("TXT_Rank%d"), LeaderboardsList[i].Rank);
		const FName NameTextBlockName = *FString::Printf(TEXT("TXT_Name%d"), LeaderboardsList[i].Rank);
		const FName ScoreTextBlockName = *FString::Printf(TEXT("TXT_Value%d"), LeaderboardsList[i].Rank);

		UTextBlock* RankTextBlock = Cast<UTextBlock>(GetWidgetFromName(RankTextBlockName));
		UTextBlock* NameTextBlock = Cast<UTextBlock>(GetWidgetFromName(NameTextBlockName));
		UTextBlock* ScoreTextBlock = Cast<UTextBlock>(GetWidgetFromName(ScoreTextBlockName));
		if (RankTextBlock && NameTextBlock && ScoreTextBlock)
		{
			RankTextBlockLookup.Add(RankTextBlock);
			NameTextBlockLookup.Add(NameTextBlock);
			ValueTextBlockLookup.Add(ScoreTextBlock);
		}
	}

	if (GI && !LeaderboardsList.IsEmpty())
	{
		SetLeaderboards();
	}
}

void UIRRankKillEnemyWidget::OnStatisticsClick()
{
	RemoveFromParent();

	AIRUIPlayerController* PlayerController = Cast<AIRUIPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		PlayerController->OnDisplayWidget(EWidgetType::Statistics);
	}
}

void UIRRankKillEnemyWidget::OnRankStageClick()
{
	RemoveFromParent();

	AIRUIPlayerController* PlayerController = Cast<AIRUIPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		PlayerController->OnDisplayWidget(EWidgetType::RankStage);
	}
}

void UIRRankKillEnemyWidget::OnCloseClick()
{
	AIRUIPlayerController* PlayerController = Cast<AIRUIPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		PlayerController->EnableButtons();
	}

	RemoveFromParent();
}

void UIRRankKillEnemyWidget::SetLeaderboards()
{
	for (int i = 0; i < LeaderboardsList.Num(); ++i)
	{
		RankTextBlockLookup[i]->SetText(FText::FromString(FString::Printf(TEXT("#%d"), LeaderboardsList[i].Rank)));
		NameTextBlockLookup[i]->SetText(FText::FromString(*LeaderboardsList[i].Name));
		ValueTextBlockLookup[i]->SetText(FText::FromString(FString::FromInt(LeaderboardsList[i].Score)));

		if (MyLeaderboard.Rank == LeaderboardsList[i].Rank)
		{
			FLinearColor SelectedColor = FLinearColor::Yellow;
			RankTextBlockLookup[i]->SetColorAndOpacity(SelectedColor);
			NameTextBlockLookup[i]->SetColorAndOpacity(SelectedColor);
			ValueTextBlockLookup[i]->SetColorAndOpacity(SelectedColor);
		}
	}

	TXT_MyRank->SetText(FText::FromString(FString::Printf(TEXT("#%d"), MyLeaderboard.Rank)));
	TXT_MyName->SetText(FText::FromString(*MyLeaderboard.Name));
	TXT_MyValue->SetText(FText::FromString(FString::FromInt(MyLeaderboard.Score)));
}
