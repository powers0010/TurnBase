// Fill out your copyright notice in the Description page of Project Settings.

#include "UIHPWidget.h"
#include "Character/BaseBattlePawn/BaseBattlePawn.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UUIHPWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (owner)
	{
		NameTxt = Cast<UTextBlock>(GetWidgetFromName(TEXT("NameTxt")));
		SetNameTxt(owner->GetAttribute().PawnName);

		HPTxt = Cast<UTextBlock>(GetWidgetFromName(TEXT("HPTxt")));
		SetHPTxt(owner->GetAttribute().CurHP, owner->GetAttribute().MaxHP);

		ArrowImage = Cast<UImage>(GetWidgetFromName(TEXT("Arrow")));

		SelectImage = Cast<UImage>(GetWidgetFromName(TEXT("Selected")));
	}
}

void UUIHPWidget::SetNameVisible(bool NewVis)
{
	if (NameTxt)
	{
		NameTxt->SetVisibility(NewVis ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}
void UUIHPWidget::SetHPVisible(bool NewVis)
{
	if (HPTxt)
	{
		HPTxt->SetVisibility(NewVis ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void UUIHPWidget::SetArrowVisible(bool NewVis)
{
	if (ArrowImage)
	{
		ArrowImage->SetVisibility(NewVis ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void UUIHPWidget::SetSelectedImageVisible(bool NewVis)
{
	if (SelectImage)
	{
		SelectImage->SetVisibility(NewVis ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void UUIHPWidget::SetHPTxt(float HP, float MaxHP)
{
	if (HPTxt)
	{
		FText hptxt = FText::FromString(FString::Printf(TEXT("%d/%d"), (int32)HP, (int32)MaxHP)) ;
		HPTxt->SetText(hptxt);
	}
}

void UUIHPWidget::SetNameTxt(FText name)
{
	if (NameTxt)
	{
		NameTxt->SetText(name);
	}
}