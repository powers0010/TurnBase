// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseBattleNPC.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "Globe/CustomMSG.h"
#include "UI/UIHPWidget.h"
#include "Controller/TurnBasePlayerController.h"
#include "Actor/BaseBattleLevelActor.h"
#include "GamePlay/MyGameInstance.h"
#include "Globe/DataAssetManager.h"
#include "Character/Anim/BaseAnimInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "UI/UIBattleWidget.h"
#include "Actor/BaseBattleLevelActor.h"



void ABaseBattleNPC::InitPawnFromData(int32 id)
{
	UMyGameInstance* instance = Cast<UMyGameInstance>(GetGameInstance());
	if (instance && instance->DataManager)
	{
		FPawnAttributeData* PawnAttributeMsg = instance->DataManager->GetPawnAttributeAsset(id);
		if (PawnAttributeMsg)
		{
			PawnName = PawnAttributeMsg->PawnName;
			CurATK = PawnAttributeMsg->ATK;
			CurMagicATK = PawnAttributeMsg->MagicATK;
			CurDefense = PawnAttributeMsg->Defense;
			CurMagicDefense = PawnAttributeMsg->MagicDefense;
			CurBattleRateSpeed = PawnAttributeMsg->BattleRateSpeed;
			CurHP = MaxHP = PawnAttributeMsg->MaxHP;
			CurLevel = 1;
			FightSeqImage = LoadObject<UTexture2D>(NULL, *PawnAttributeMsg->Image_FightSeq);

			USkeletalMesh* SkeletalMesh = LoadObject<USkeletalMesh>(NULL, *PawnAttributeMsg->MeshPath);
			if (SkeletalMesh)
			{
				GetMesh()->SetSkeletalMesh(SkeletalMesh);
			}
			UClass* AnimBPClass = LoadClass<UBaseAnimInstance>(NULL, *PawnAttributeMsg->AnimBPPath);
			if (AnimBPClass)
			{
				GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
				GetMesh()->SetAnimInstanceClass(AnimBPClass);
			}
			AnimSeq_Die = LoadObject<UAnimSequence>(NULL, *PawnAttributeMsg->AnimSeq_Die);
			AnimSeq_Win = LoadObject<UAnimSequence>(NULL, *PawnAttributeMsg->AnimSeq_Win);
			AnimMon_GetHit = LoadObject<UAnimMontage>(NULL, *PawnAttributeMsg->AnimMon_GetHit);
			AnimMon_Attack = LoadObject<UAnimMontage>(NULL, *PawnAttributeMsg->AnimMon_Attack);

			//SkillData
			TArray<int32> Skillids;
			int32 idgroup = PawnAttributeMsg->SkillID;
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