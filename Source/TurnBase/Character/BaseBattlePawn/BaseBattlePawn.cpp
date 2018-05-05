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
	GetCharacterMovement()->bOrientRotationToMovement = true;

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

	if (bNeedMove)
	{
		FVector Dir = (MoveTargetLoc - MoveOrign).GetSafeNormal();
		AddMovementInput(Dir);
		
		if (FVector::DotProduct((GetActorLocation()- MoveTargetLoc), Dir) > 0.f)
		{
			SetActorLocation(MoveTargetLoc);
			bNeedMove = false;
			if (CurLevelActor)
			{
				CurLevelActor->OnMoveSuccess(bIsMoveToAttacking);
			}
		}
	}

	if (bNeedRot)
	{
		FRotator nextrot = GetActorRotation();
		nextrot.Yaw += DeYaw *DeltaTime * 4.f;

		if ((TargetRot.Yaw - nextrot.Yaw)*DeYaw < 0.f)
		{
			SetActorRotation(TargetRot);
			bNeedRot = false;
		}
		else
		{
			SetActorRotation(nextrot);
		}
	}
}

void ABaseBattlePawn::MoveToTarget(FVector& TargetLoc, bool bIsToAttacking)
{
	MoveOrign = GetActorLocation();
	MoveTargetLoc = TargetLoc;
	bIsMoveToAttacking = bIsToAttacking;
	bNeedMove = true;
}

void ABaseBattlePawn::RotatorToTargetRotator(FRotator& Rot)
{
	TargetRot = Rot;
	DeYaw = Rot.Yaw - GetActorRotation().Yaw;
	bNeedRot = true;
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
		if (DamageType == 0) //ÎïÀíÉËº¦
		{
			damage = Attacker->GetCurATK() * skillinfo.Damage - GetCurDefence();
		}
		else if(DamageType == 1) //Ä§·¨ÉËº¦
		{
			damage = Attacker->GetCurMagicATK() * skillinfo.Damage - GetCurMagicDefence();
		}
		
		damage = FMath::Max<float>(0.f, damage) * (bIsInDefense ? 0.3f : 1.f);
		DecCurHP(damage);
		
		if (GetCurHP() <= 0.f)
		{
			SetCurHP(0.f);
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
			HPWidget->SetHPTxt(GetCurHP(), GetMaxHP());
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
