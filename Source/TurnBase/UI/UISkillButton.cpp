// Fill out your copyright notice in the Description page of Project Settings.

#include "UISkillButton.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Actor/BaseBattleLevelActor.h"
#include "UI/UIBattleWidget.h"



void UUISkillButton::NativeConstruct()
{
	Super::NativeConstruct();

	SkillBtn = Cast<UButton>(GetWidgetFromName(TEXT("SkillBtn")));
	SkillBtn->OnClicked.AddDynamic(this, &UUISkillButton::OnSkillbtnClick);

	SkillTxt = Cast<UTextBlock>(GetWidgetFromName(TEXT("SkillName")));
}

void UUISkillButton::OnSkillbtnClick()
{
	if (LecelActor)
	{
		LecelActor->ClearCurDamageEnemys();
		LecelActor->SetIsInEnemySelect(true);
		LecelActor->CurSkillID = SkillId;
		if (LecelActor->GetBattleUI())
		{
			LecelActor->GetBattleUI()->SetSkillOrderVisible(false);
		}
	}
}

void UUISkillButton::SetSkillName(FText name)
{
	if (SkillTxt)
	{
		SkillTxt->SetText(name);
	}
}