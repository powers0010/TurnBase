// Fill out your copyright notice in the Description page of Project Settings.

#include "UIFightSeqWidget.h"
#include "Components/Image.h"



void UUIFightSeqWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PawnIcon = Cast<UImage>(GetWidgetFromName(TEXT("PawnImage")));
}

void UUIFightSeqWidget::SetPawnIcon(UTexture2D* image)
{
	if (PawnIcon && image)
	{
		PawnIcon->SetBrushFromTexture(image);
	}
}