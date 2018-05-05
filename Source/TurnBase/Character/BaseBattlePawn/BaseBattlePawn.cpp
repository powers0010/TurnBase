// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseBattlePawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
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


// Sets default values
ABaseBattlePawn::ABaseBattlePawn()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(CapsuleSize.X, CapsuleSize.Y);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetRelativeScale3D(MeshScale);
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -CapsuleSize.Y));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f,0.f));

	GetCharacterMovement()->MaxWalkSpeed = 200.f;

	WidgetComComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPWidgetCom"));
	WidgetComComponent->SetupAttachment(GetCapsuleComponent());
	WidgetComComponent->SetRelativeLocation(FVector(0.f, 0.f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));
	WidgetComComponent->SetWidgetSpace(EWidgetSpace::Screen);
	WidgetComComponent->SetDrawAtDesiredSize(true);
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Game/Blueprint/UI/BPW_HP.BPW_HP_C"));
	if (WidgetClass.Class != NULL)
	{
		WidgetComComponent->SetWidgetClass(WidgetClass.Class);
	}
}

// Called when the game starts or when spawned
void ABaseBattlePawn::BeginPlay()
{
	Super::BeginPlay();
	
	if (WidgetComComponent)
	{
		HPWidget = Cast<UUIHPWidget>(WidgetComComponent->GetUserWidgetObject());
		if (HPWidget)
		{
			HPWidget->owner = this;
		}
	}
}

// Called every frame
void ABaseBattlePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseBattlePawn::InitPawnFromData(int32 id)
{
	UMyGameInstance* instance = Cast<UMyGameInstance>(GetGameInstance());
	if (instance && instance->DataManager)
	{
		FAttributeData* HeroAttributeMsg = instance->DataManager->GetAttributeAsset(id);
		if (HeroAttributeMsg)
		{
			Attribute.PawnName = HeroAttributeMsg->PawnName;
			Attribute.CurATK = Attribute.ATK = HeroAttributeMsg->ATK;
			Attribute.CurMagicATK = Attribute.MagicATK = HeroAttributeMsg->MagicATK;
			Attribute.CurDefense = Attribute.Defense = HeroAttributeMsg->Defense;
			Attribute.CurMagicDefense = Attribute.MagicDefense = HeroAttributeMsg->MagicDefense;
			Attribute.CurBattleRateSpeed = Attribute.BattleRateSpeed = HeroAttributeMsg->BattleRateSpeed;
			Attribute.CurHP = Attribute.MaxHP = HeroAttributeMsg->MaxHP;
			Attribute.CurLevel = 1;
			Attribute.FightSeqImage = LoadObject<UTexture2D>(NULL, *HeroAttributeMsg->Image_FightSeq);

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
			for (int32 i =0;i<Skillids.Num(); i++)
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

bool ABaseBattlePawn::GetSkillInfo(int32 SkillID, FSkillInfo& info)
{
	if (SkillID == 1)
	{
		info = FSkillInfo(AnimMon_Attack, 1.f);
		return true;
	}
	else if (SkillInfos.Contains(SkillID))
	{
		info = SkillInfos[SkillID];
		return true;
	}
	return false;
}

float ABaseBattlePawn::DoAttack(int32 SkillID)
{
	if (SkillID == 1)
	{
		return PlayAnimMontage(AnimMon_Attack);
	}
	else if (SkillInfos.Contains(SkillID))
	{
		return PlayAnimMontage(SkillInfos[SkillID].AnimMon_Skill);
	}
	return 0.f;
}

void ABaseBattlePawn::HandleDamage(ABaseBattlePawn* Attacker, FSkillInfo& skillinfo)
{
	if (bIsAlive)
	{
		int32 DamageType = skillinfo.DamageType;
		float damage = 0.f;
		if (DamageType == 0) //�����˺�
		{
			damage = Attacker->Attribute.CurATK * skillinfo.Damage - Attribute.CurDefense;
		}
		else if(DamageType == 1) //ħ���˺�
		{
			damage = Attacker->Attribute.CurMagicATK * skillinfo.Damage - Attribute.CurMagicDefense;
		}
		
		damage = FMath::Max<float>(0.f, damage) * (bIsInDefense ? 0.3f : 1.f);
		Attribute.CurHP -= damage;
		if (Attribute.CurHP <= 0.f)
		{
			Attribute.CurHP = 0.f;
			Die();
		}
		else
		{
			if (AnimMon_GetHit)
			{
				PlayAnimMontage(AnimMon_GetHit);
			}
		}
		if (HPWidget)
		{
			HPWidget->SetHPTxt(Attribute.CurHP, Attribute.MaxHP);
		}
	}	
}

void ABaseBattlePawn::Die()
{
	bIsAlive = false;
	if (AnimSeq_Die)
	{
		GetMesh()->PlayAnimation(AnimSeq_Die, false);
	}
	if (HPWidget)
	{
		HPWidget->SetNameVisible(false);
		HPWidget->SetHPVisible(false);
	}
	if (CurLevelActor && CurLevelActor->GetBattleUI())
	{
		CurLevelActor->GetBattleUI()->SetFightSeqImageVisible(this, false);
	}
}

int32 ABaseBattlePawn::AISelectSkill()
{
	return 1;
}

void ABaseBattlePawn::OnWin()
{
	if (AnimSeq_Win)
	{
		GetMesh()->PlayAnimation(AnimSeq_Win, true);
	}
}