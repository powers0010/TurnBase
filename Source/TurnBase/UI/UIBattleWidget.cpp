// Fill out your copyright notice in the Description page of Project Settings.

#include "UIBattleWidget.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "Character/BaseBattlePawn/BaseBattlePawn.h"
#include "UI/UIFightSeqWidget.h"
#include "Runtime/UMG/Public/Blueprint/WidgetTree.h"
#include "Controller/TurnBasePlayerController.h"
#include "Actor/BaseBattleLevelActor.h"
#include "UI/UISkillButton.h"


void UUIBattleWidget::NativeConstruct()
{
	Super::NativeConstruct();

	AtkBtn = Cast<UButton>(GetWidgetFromName(TEXT("Attack")));
	AtkBtn->OnClicked.AddDynamic(this, &UUIBattleWidget::OnAttackBthClick);

	SkillBtn = Cast<UButton>(GetWidgetFromName(TEXT("Skill")));
	SkillBtn->OnClicked.AddDynamic(this, &UUIBattleWidget::OnSkillBtnClick);

	DefenseBtn = Cast<UButton>(GetWidgetFromName(TEXT("Defense")));
	DefenseBtn->OnClicked.AddDynamic(this, &UUIBattleWidget::OnDefenseBtnClick);

	WinBtn = Cast<UButton>(GetWidgetFromName(TEXT("WinButton")));
	WinBtn->OnClicked.AddDynamic(this, &UUIBattleWidget::OnWinBthClick);

	LoseBtn = Cast<UButton>(GetWidgetFromName(TEXT("LoseButton")));
	LoseBtn->OnClicked.AddDynamic(this, &UUIBattleWidget::OnWinBthClick);

	AttackOrderPanel = Cast<UVerticalBox>(GetWidgetFromName(TEXT("AttackOrder")));
	SkillOrderPanel = Cast<UVerticalBox>(GetWidgetFromName(TEXT("SkillOrder")));
	FightSeqCanvas = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("FightSeqPanel")));

	FightSeqClass = LoadClass<UUIFightSeqWidget>(NULL, TEXT("/Game/Blueprint/UI/BPW_FightSeqWidget.BPW_FightSeqWidget_C"));
	SkillBtnClass = LoadClass<UUISkillButton>(NULL, TEXT("/Game/Blueprint/UI/BPW_SkillBtn.BPW_SkillBtn_C"));
}

void UUIBattleWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}

void UUIBattleWidget::OnBattleEnd()
{
	for (auto It = FightSeqImages.CreateConstIterator(); It; ++It)
	{
		if (It.Value())
		{
			It.Value()->RemoveFromParent();
		}	
	}
	FightSeqImages.Empty();

	ClearSkillBorder();
}

void UUIBattleWidget::OnAttackEnd()
{
	ClearSkillBorder();
	bNeedInitSkillBorder = true;
}

void UUIBattleWidget::ClearSkillBorder()
{
	if (SkillOrderPanel)
	{
		SkillOrderPanel->ClearChildren();
	}
}

void UUIBattleWidget::SetAttackOrderVisible(bool NewVis)
{
	if (AttackOrderPanel)
	{
		AttackOrderPanel->SetVisibility(NewVis ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void UUIBattleWidget::SetSkillOrderVisible(bool NewVis)
{
	if (SkillOrderPanel)
	{
		SkillOrderPanel->SetVisibility(NewVis ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void UUIBattleWidget::SetWinBorderVisible(bool NewVis)
{
	if (WinBtn)
	{
		WinBtn->SetVisibility(NewVis ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void UUIBattleWidget::SetLoseBorderVisible(bool NewVis)
{
	if (LoseBtn)
	{
		LoseBtn->SetVisibility(NewVis ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void UUIBattleWidget::OnAttackBthClick()
{
	if (LecelActor)
	{
		LecelActor->ClearCurDamageEnemys();
		LecelActor->SetIsInEnemySelect(true);
		LecelActor->CurSkillID = 1;
	}
}

void UUIBattleWidget::OnSkillBtnClick()
{
	if (SkillOrderPanel && SkillBtnClass && LecelActor && LecelActor->GetCurAttacker())
	{
		SkillOrderPanel->SetVisibility(ESlateVisibility::Visible);
		if (bNeedInitSkillBorder)
		{
			TMap<int32, FSkillInfo>& info = LecelActor->GetCurAttacker()->GetSkillInfos();
			for (auto It = info.CreateConstIterator(); It; ++It)
			{
				UUISkillButton* btn = CreateWidget<UUISkillButton>(GetWorld(), SkillBtnClass);
				if (btn)
				{
					SkillOrderPanel->AddChildToVerticalBox(btn);
					btn->SkillId = It.Value().Id;
					btn->LecelActor = LecelActor;
					btn->SetSkillName(It.Value().ViewName);
				}
			}
			bNeedInitSkillBorder = false;
		}		
	}
}

void UUIBattleWidget::OnDefenseBtnClick()
{
	if (LecelActor)
	{
		LecelActor->OnDefense();
		if (AttackOrderPanel)
		{
			AttackOrderPanel->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UUIBattleWidget::OnWinBthClick()
{
	if (LecelActor)
	{
		LecelActor->BattleEnd();
	}
}

void UUIBattleWidget::AddFightSeqImage(class ABaseBattlePawn* OnPawn)
{
	if (FightSeqCanvas && FightSeqClass)
	{
		if (OnPawn && OnPawn->Attribute.FightSeqImage)
		{
			UUIFightSeqWidget* img = NewObject<UUIFightSeqWidget>(this, FightSeqClass);
			if (img)
			{				
				FightSeqCanvas->AddChildToCanvas(img);
				img->SetAlignmentInViewport(FVector2D(0.f, 0.f));				
				img->SetPawnIcon(OnPawn->Attribute.FightSeqImage);
				FightSeqImages.Add(OnPawn, img);
			}
		}
	}
}

void UUIBattleWidget::SetFightSeqImageOffset(ABaseBattlePawn* bpawn, float SeqPercent)
{
	if (FightSeqCanvas && bpawn)
	{
		if (FightSeqImages.Contains(bpawn) && FightSeqImages[bpawn])
		{
			float offset = SeqPercent * FightSeqCanvas->GetDesiredSize().X;
			FightSeqImages[bpawn]->SetRenderTranslation(FVector2D(offset, 0.f));
		}
	}
}

void UUIBattleWidget::SetFightSeqImageVisible(ABaseBattlePawn* bpawn, bool NewVis)
{
	if (bpawn && FightSeqImages.Contains(bpawn) && FightSeqImages[bpawn])
	{
		FightSeqImages[bpawn]->SetVisibility(NewVis ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}