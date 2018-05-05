// Fill out your copyright notice in the Description page of Project Settings.

#include "BasePlayerPawn.h"
#include "Components/SkeletalMeshComponent.h"
#include "UI/UIHPWidget.h"
#include "GamePlay/MyGameInstance.h"
#include "Globe/DataAssetManager.h"
#include "Globe/CustomMSG.h"
#include "Character/Anim/BaseAnimInstance.h"

void ABasePlayerPawn::InitPawnFromData(int32 id)
{
	UMyGameInstance* instance = Cast<UMyGameInstance>(GetGameInstance());
	if (instance && instance->DataManager)
	{
		FHeroAttributeData* HeroAttributeMsg = instance->DataManager->GetHeroAttributeAsset(id);
		if (HeroAttributeMsg)
		{
			PawnName = HeroAttributeMsg->PawnName;
			CurATK = ATK = HeroAttributeMsg->ATK;
			CurMagicATK = MagicATK = HeroAttributeMsg->MagicATK;
			CurDefense = Defense = HeroAttributeMsg->Defense;
			CurMagicDefense = MagicDefense = HeroAttributeMsg->MagicDefense;
			CurBattleRateSpeed = BattleRateSpeed = HeroAttributeMsg->BattleRateSpeed;
			CurHP = MaxHP = HeroAttributeMsg->MaxHP;
			CurLevel = 1;
			CurLevelMaxEXP = LevelEXP::CaculatLevelEXP(CurLevel);
			FightSeqImage = LoadObject<UTexture2D>(NULL, *HeroAttributeMsg->Image_FightSeq);

			USkeletalMesh* SkeletalMesh = LoadObject<USkeletalMesh>(NULL, *HeroAttributeMsg->MeshPath);
			if (SkeletalMesh)
			{
				GetMesh()->SetSkeletalMesh(SkeletalMesh);
			}
			UClass* AnimBPClass = LoadClass<UBaseAnimInstance>(NULL, *HeroAttributeMsg->AnimBPPath);
			if (AnimBPClass)
			{
				GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
				GetMesh()->SetAnimInstanceClass(AnimBPClass);
			}
			AnimSeq_Die = LoadObject<UAnimSequence>(NULL, *HeroAttributeMsg->AnimSeq_Die);
			AnimSeq_Win = LoadObject<UAnimSequence>(NULL, *HeroAttributeMsg->AnimSeq_Win);
			AnimMon_GetHit = LoadObject<UAnimMontage>(NULL, *HeroAttributeMsg->AnimMon_GetHit);
			AnimMon_Attack = LoadObject<UAnimMontage>(NULL, *HeroAttributeMsg->AnimMon_Attack);

			//SkillData
			TArray<int32> Skillids;
			int32 idgroup = HeroAttributeMsg->SkillID;
			while (idgroup > 1000)
			{
				int32 spid = idgroup % 10000;
				Skillids.Add(spid);
				idgroup = (int32)(idgroup / 10000);
			}
			for (int32 i = 0; i < Skillids.Num(); i++)
			{
				FSkillInfo* Skillinfo = instance->DataManager->GetSkillData(Skillids[i]);
				if (Skillinfo)
				{
					Skillinfo->AnimMon_Skill = LoadObject<UAnimMontage>(NULL, *Skillinfo->AnimPath);
					SkillInfos.Add(Skillids[i], *Skillinfo);
				}
			}
		}
	}
}

void ABasePlayerPawn::OnWin(float GainExp)
{
	Super::OnWin(GainExp);

	GainEXP(GainExp);
}

void ABasePlayerPawn::ResetForBattleEnd(FVector loc)
{
	SetActorLocation(loc);
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	if (!bIsAlive)
	{
		bIsAlive = true;
		CurHP = 1.f;
		if (HPWidget)
		{
			HPWidget->SetHPTxt(CurHP, MaxHP);
			HPWidget->SetHPVisible(true);
			HPWidget->SetNameVisible(true);
		}
	}
}

void ABasePlayerPawn::GainEXP(float InExp)
{
	CurEXP += InExp;
	if (CurEXP > CurLevelMaxEXP)
	{
		OnLevelUp(CurEXP - CurLevelMaxEXP);
	}
}

void ABasePlayerPawn::OnLevelUp(float OverExp)
{
	CurEXP = 0.f;
	CurLevel++;
	CurLevelMaxEXP = LevelEXP::CaculatLevelEXP(CurLevel);
	if (HPWidget)
	{
		HPWidget->SetNameTxt(GetViewName());
	}
	GainEXP(OverExp);
}