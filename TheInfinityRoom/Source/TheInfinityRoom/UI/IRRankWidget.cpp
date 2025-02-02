// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/IRRankWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Player/IRUIPlayerController.h"

void UIRRankWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (nullptr != BTN_Statistics)
	{
		BTN_Statistics->OnClicked.AddDynamic(this, &UIRRankWidget::OnStatisticsClick);
	}

	if (nullptr != BTN_RankKill)
	{
		BTN_RankKill->OnClicked.AddDynamic(this, &UIRRankWidget::OnRankKillClick);
	}

	if (nullptr != BTN_Close)
	{
		BTN_Close->OnClicked.AddDynamic(this, &UIRRankWidget::OnCloseClick);
	}

	UIRGameInstance* GI = Cast<UIRGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI)
	{
		TMap<int32, FLeaderboardRow> LeaderboardMap = GI->GetLeaderboards(true);
		for (auto Pair : LeaderboardMap)
		{
			LeaderboardsList.Add(Pair.Value);
		}

		TMap<int32, FLeaderboardRow> MyLeaderboardMap = GI->GetMyLeaderboards();
		MyLeaderboard = *MyLeaderboardMap.Find(0);
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

void UIRRankWidget::OnStatisticsClick()
{
	RemoveFromParent();

	AIRUIPlayerController* PlayerController = Cast<AIRUIPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		PlayerController->OnDisplayWidget(EWidgetType::Statistics);
	}
}

void UIRRankWidget::OnRankKillClick()
{
	RemoveFromParent();

	AIRUIPlayerController* PlayerController = Cast<AIRUIPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		PlayerController->OnDisplayWidget(EWidgetType::RankKill);
	}
}

void UIRRankWidget::OnCloseClick()
{
	AIRUIPlayerController* PlayerController = Cast<AIRUIPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		PlayerController->EnableButtons();
	}

	RemoveFromParent();
}

void UIRRankWidget::SetLeaderboards()
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
