// Fill out your copyright notice in the Description page of Project Settings.

#include "BasePlayerPawn.h"
#include "Components/SkeletalMeshComponent.h"
#include "UI/UIHPWidget.h"


void ABasePlayerPawn::ResetForBattleEnd(FVector loc)
{
	SetActorLocation(loc);
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	if (!bIsAlive)
	{
		bIsAlive = true;
		Attribute.CurHP = 1.f;
		if (HPWidget)
		{
			HPWidget->SetHPTxt(Attribute.CurHP, Attribute.MaxHP);
			HPWidget->SetHPVisible(true);
			HPWidget->SetNameVisible(true);
		}
	}
}
