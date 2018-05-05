// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseBattleLevelActor.h"
#include "Character/TurnBaseCharacter.h"
#include "Character/BaseBattlePawn/BaseBattlePawn.h"
#include "Character/BaseEnemy/BaseEnemy.h"
#include "Controller/TurnBasePlayerController.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "UI/UIHPWidget.h"
#include "UI/UIBattleWidget.h"
#include "AI/Navigation/NavigationSystem.h"
#include "Controller/BaseBattlePawnController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/BaseBattlePawn/BasePlayerPawn.h"


const int32  SeatingNum = 6;



// Sets default values
ABaseBattleLevelActor::ABaseBattleLevelActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	LevelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LevelMesh"));
	LevelMesh->SetRelativeLocation(FVector(0.f, 0.f, -5.f));
	LevelMesh->SetRelativeScale3D(FVector(4.f));
	LevelMesh->SetupAttachment(RootComponent);


	BattleLevelCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("BattleLevelCamera"));
	BattleLevelCamera->SetupAttachment(RootComponent);
	BattleLevelCamera->SetRelativeRotation(FRotator(-35.f, 50.f, 0.f));
	BattleLevelCamera->SetRelativeLocation(FVector(-380.f, -380.f, 350.f));

	LevelType = EBattleLevelType::EBT_BiWuTai;

	RedArrowLoc.Add(FVector(-120.f, 0.f, 0.f));
	RedArrowLoc.Add(FVector(-120.f, -150.f, 0.f));
	RedArrowLoc.Add(FVector(-120.f, 150.f, 0.f));
	RedArrowLoc.Add(FVector(-300.f, 0.f, 0.f));
	RedArrowLoc.Add(FVector(-300.f, -150.f, 0.f));
	RedArrowLoc.Add(FVector(-300.f, 150.f, 0.f));

	BlueArrowLoc.Add(FVector(120.f, 0.f, 0.f));
	BlueArrowLoc.Add(FVector(120.f, -150.f, 0.f));
	BlueArrowLoc.Add(FVector(120.f, 150.f, 0.f));
	BlueArrowLoc.Add(FVector(300.f, 0.f, 0.f));
	BlueArrowLoc.Add(FVector(300.f, -150.f, 0.f));
	BlueArrowLoc.Add(FVector(300.f, 150.f, 0.f));

	for (int32 i = 0; i < RedArrowLoc.Num(); i++)
	{
		UArrowComponent* arrow = CreateDefaultSubobject<UArrowComponent>(*FString::Printf(TEXT("RedArrow_%d"), i));
		arrow->SetupAttachment(RootComponent);
		arrow->SetRelativeLocation(RedArrowLoc[i]);
		RedGroup.Add(arrow);
	}
	for (int32 i = 0; i < BlueArrowLoc.Num(); i++)
	{
		UArrowComponent* arrow = CreateDefaultSubobject<UArrowComponent>(*FString::Printf(TEXT("BlueArrow_%d"), i));
		arrow->SetupAttachment(RootComponent);
		arrow->SetRelativeLocation(BlueArrowLoc[i]);
		arrow->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));
		BlueGroup.Add(arrow);
	}	
}

// Called when the game starts or when spawned
void ABaseBattleLevelActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (!BattleUI)
	{
		UClass* BattleUIClass = LoadClass<UUIBattleWidget>(NULL, TEXT("/Game/Blueprint/UI/BPW_BattleWidget.BPW_BattleWidget_C"));
		if (BattleUIClass)
		{
			BattleUI = CreateWidget<UUIBattleWidget>(GetWorld(), BattleUIClass);
			if (BattleUI)
			{
				BattleUI->InitLevelActor(this);
			}
		}
	}
}

// Called every frame
void ABaseBattleLevelActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	BattleTick(DeltaTime);
	DoEnemySelect();
}

void ABaseBattleLevelActor::DoEnemySelect()
{
	if (bIsInEnemySelect && CurAttacker)
	{
		ATurnBasePlayerController* TSC = Cast<ATurnBasePlayerController>(GetWorld()->GetFirstPlayerController());
		if (TSC)
		{
			FVector StartTrace = FVector::ZeroVector;
			FRotator UnusedRot;
			TSC->GetPlayerViewPoint(StartTrace, UnusedRot);

			FVector MouseLoc = FVector::ZeroVector;
			FVector MouseDir = FVector::ZeroVector;
			bool bSuc = TSC->DeprojectMousePositionToWorld(MouseLoc, MouseDir);
			if (bSuc)
			{
				FHitResult result;
				FCollisionObjectQueryParams Params(ECC_Pawn);
				GetWorld()->LineTraceSingleByObjectType(result, StartTrace, StartTrace + MouseDir * 1000.f, Params);

				if (result.bBlockingHit)
				{
					ABaseBattlePawn* target = Cast<ABaseBattlePawn>(result.GetActor());
					if (target && target->bIsAlive && CurSelectEnemy != target )
					{
						if (EnemyPawns.Contains(target) && !CurDamageEnemys.Contains(target))
						{						
							ClearCurDamageEnemys();
							CurSelectEnemy = target;
							GetEnemyForDiffDamageRange();
							if (CurDamageEnemys.Num()> 0)
							{
								for (int32 i = 0; i < CurDamageEnemys.Num(); i++)
								{
									if (CurDamageEnemys[i] && CurDamageEnemys[i]->GetHPWidget())
									{
										CurDamageEnemys[i]->GetHPWidget()->SetSelectedImageVisible(true);
									}
								}
							}
						}
					}
				}
			}
		}
	}	
}

void ABaseBattleLevelActor::BattleStart(class ATurnBaseCharacter* player, class ABaseEnemy* enemy)
{
	if (player && enemy)
	{
		player->GetCharacterMovement()->MaxWalkSpeed = 0.f;
		ATurnBasePlayerController* TSC = Cast<ATurnBasePlayerController>(GetWorld()->GetFirstPlayerController());
		if (TSC)
		{
			TSC->CurBattleLevelActor = this;
			TSC->bMoveToMouseCursor = 0;

			Player = player;
			Enemy = enemy;

			bIsInBattleTick = true;
			TSC->bIsInBattleLevel = true;

			if (BattleUI)
			{
				if (BattleUI->IsInViewport())
				{
					BattleUI->SetVisibility(ESlateVisibility::Visible);
				}
				else
				{
					BattleUI->AddToViewport();
				}				
			}

			for (int32 i = 0; i < 6; i++)
			{
				if (RedGroup.Num() > i && RedGroup[i])
				{
					if (Player->GetPlayerPawn(i))
					{
						Player->GetPlayerPawn(i)->SetActorTransform(RedGroup[i]->GetComponentTransform());
						Player->GetPlayerPawn(i)->CurLevelActor = this;
						PlayerPawns.Add(Player->GetPlayerPawn(i), FPawnFightMsg(i));
						if (BattleUI)
						{
							BattleUI->AddFightSeqImage(Player->GetPlayerPawn(i));
						}
					}

				}
			}

			for (int32 i = 0; i < FMath::Min<int32>(Enemy->GetAllEnemy().Num(), SeatingNum); i++)
			{
				if (BlueGroup.Num() > i && BlueGroup[i])
				{
					ABaseBattlePawn* btp = GetWorld()->SpawnActor<ABaseBattlePawn>(ABaseBattlePawn::StaticClass(), BlueGroup[i]->GetComponentTransform());
					if (btp)
					{
						btp->InitPawnFromData(Enemy->GetAllEnemy()[i]);
						btp->CurLevelActor = this;
						ABaseBattlePawnController* control = GetWorld()->SpawnActor<ABaseBattlePawnController>(ABaseBattlePawnController::StaticClass(), RedGroup[i]->GetComponentTransform());
						if (control)
						{							
							control->Possess(btp);
							//btp->InitPawnFromData();
							EnemyPawns.Add(btp, FPawnFightMsg(i));
						}
						if (BattleUI)
						{
							BattleUI->AddFightSeqImage(btp);
						}
					}									
				}
			}
			TSC->SetViewTarget(this);
		}
	}
}

void ABaseBattleLevelActor::BattleEnd()
{
	if (Player && Enemy)
	{
		Player->GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;

		ATurnBasePlayerController* TSC = Cast<ATurnBasePlayerController>(GetWorld()->GetFirstPlayerController());
		if (TSC)
		{
			TSC->CurBattleLevelActor = nullptr;
			TSC->SetViewTarget(Player);
			TSC->bIsInBattleLevel = false;
		}

		if (BattleUI)
		{
			BattleUI->SetWinBorderVisible(false);
			BattleUI->SetLoseBorderVisible(false);
			BattleUI->SetAttackOrderVisible(false);
			BattleUI->OnBattleEnd();
			BattleUI->SetVisibility(ESlateVisibility::Hidden);
		}

		for (int32 i = 0; i < 6; i++)
		{
			if (RedGroup.Num() > i && RedGroup[i])
			{
				Player->ResetPawnsLoc();
			}
		}
		for (auto It = EnemyPawns.CreateIterator(); It; ++It)
		{
			if (It.Key())
			{
				It.Key()->Destroy();
			}
		}
		PlayerPawns.Empty();
		EnemyPawns.Empty();

		CurSelectEnemy = nullptr;
		CurAttacker = nullptr;
		Enemy = nullptr;
		Player = nullptr;
	}	
}

void ABaseBattleLevelActor::HandlePlayerWin()
{
	if (Player)
	{
		bIsInBattleTick = false;
		for (auto It = PlayerPawns.CreateConstIterator(); It; ++It)
		{
			if (It.Key()->bIsAlive)
			{
				It.Key()->OnWin();
			}
		}
		if (BattleUI)
		{
			BattleUI->SetWinBorderVisible(true);
		}
	}
}

void ABaseBattleLevelActor::HandlePlayerLose()
{
	if (Player)
	{
		bIsInBattleTick = false;
		if (BattleUI)
		{
			BattleUI->SetLoseBorderVisible(true);
		}
	}
}

void ABaseBattleLevelActor::BattleTick(float DeltaTime)
{
	if (bIsInBattleTick)
	{
		for ( auto It = PlayerPawns.CreateIterator(); It ; ++It)
		{
			if (It.Key()->bIsAlive)
			{
				It.Value().FightSeq += It.Key()->GetAttribute().CurBattleRateSpeed * DeltaTime;
				if (It.Value().FightSeq > 100.f)
				{
					It.Value().FightSeq = 0.f;
					bIsInBattleTick = false;
					PlayerSelectEnemy(It.Key());

					break;
				}
				BattleUI->SetFightSeqImageOffset(It.Key(), It.Value().FightSeq / 100.f);
			}			
		}
	}
	if (bIsInBattleTick)
	{
		for (auto It = EnemyPawns.CreateIterator(); It; ++It)
		{
			if (It.Key()->bIsAlive)
			{
				It.Value().FightSeq += It.Key()->GetAttribute().CurBattleRateSpeed * DeltaTime;
 				if (It.Value().FightSeq > 100.f)
 				{
 					It.Value().FightSeq = 0.f;
 					bIsInBattleTick = false;
					AISelectEnemy(It.Key());
 					break;
 				}
				BattleUI->SetFightSeqImageOffset(It.Key(), It.Value().FightSeq / 100.f);
			}
		}
	}
}

void ABaseBattleLevelActor::PlayerSelectEnemy(class ABaseBattlePawn* Attacker)
{
	if (Attacker)
	{
		CurAttacker = Attacker;
		CurAttacker->bIsInDefense = false;
		if (Attacker->GetHPWidget())
		{
			Attacker->GetHPWidget()->SetArrowVisible(true);
		}
		if (BattleUI)
		{
			BattleUI->SetAttackOrderVisible(true);
		}
	}
}

void ABaseBattleLevelActor::AISelectEnemy(ABaseBattlePawn* Attacker)
{
	if (Attacker)
	{
		CurAttacker = Attacker;

		TArray<ABaseBattlePawn*> targets;
		for (auto It = PlayerPawns.CreateConstIterator(); It; ++It)
		{
			if (It.Key()->bIsAlive)
			{
				targets.Add(It.Key());
			}
		}

		if (targets.Num()>0)
		{
			int32 randindex = FMath::Rand() % targets.Num();
			CurSelectEnemy = targets[randindex];
			CurSkillID = CurAttacker->AISelectSkill();

			GetEnemyForDiffDamageRange();

			MoveToAttackerTargetLoc();
		
		}
	}
}

void ABaseBattleLevelActor::PlayerDoAttack()
{
	if (CurAttacker && CurSelectEnemy && bIsInEnemySelect)
	{
		bIsInEnemySelect = false;
		if (BattleUI)
		{
			BattleUI->SetAttackOrderVisible(false);
		}

		if (CurSkillInfo.bIsLongDisSkill)
		{
			DoAttack(CurSkillID);
		}
		else
		{
			MoveToAttackerTargetLoc();
		}		
	}
}

void ABaseBattleLevelActor::MoveToAttackerTargetLoc()
{
	if(CurAttacker && CurSelectEnemy)
	{
		AController* Contr = (CurAttacker->GetController());
		if (Contr)
		{
			FVector Tarloc = CurSelectEnemy->GetActorLocation();
			if (CurDamageEnemys.Num() > 1)
			{
				if (PlayerPawns.Contains(CurAttacker))
				{
					int32 CurSkillDamageRange = CurSkillInfo.DamageRange;
					if (CurSkillDamageRange == 1)
					{
						Tarloc = BlueGroup[CurRowOrColumn * 3]->GetComponentLocation();
					}
					else if (CurSkillDamageRange == 2)
					{
						Tarloc = BlueGroup[CurRowOrColumn]->GetComponentLocation();
					}
					else if (CurSkillDamageRange == 3)
					{
						Tarloc = BlueGroup[0]->GetComponentLocation();
					}
				}
				else if(EnemyPawns.Contains(CurAttacker))
				{
					int32 CurSkillDamageRange = CurSkillInfo.DamageRange;
					if (CurSkillDamageRange == 1)
					{
						Tarloc = RedGroup[CurRowOrColumn * 3]->GetComponentLocation();
					}
					else if (CurSkillDamageRange == 2)
					{
						Tarloc = RedGroup[CurRowOrColumn]->GetComponentLocation();
					}
					else if (CurSkillDamageRange == 3)
					{
						Tarloc = RedGroup[0]->GetComponentLocation();
					}
				}
			}
			CurAttackerMoveTarget = Tarloc - 80.f*CurAttacker->GetActorForwardVector();
			CurAttackerMoveTarget.Z = CurAttacker->GetActorLocation().Z;

			UNavigationSystem* const NavSys = GetWorld()->GetNavigationSystem();
			if (NavSys)
			{
				GetWorld()->GetTimerManager().SetTimer(CheckForAttackerMoveTimer, this, &ABaseBattleLevelActor::CheckForAttackerMoveToTargetLoc, 0.1f, true);
				NavSys->SimpleMoveToLocation(Contr, CurAttackerMoveTarget);
			}
		}
	}	
}

void ABaseBattleLevelActor::CheckForAttackerMoveToTargetLoc()
{
	if (CurAttacker)
	{
		if (CurAttacker->GetVelocity().Size() == 0.f /*(CurAttackerMoveTarget - CurAttacker->GetActorLocation()).Size() < 0.5f*/)
		{			
			GetWorld()->GetTimerManager().ClearTimer(CheckForAttackerMoveTimer);

			if (PlayerPawns.Contains(CurAttacker))
			{
				CurAttacker->SetActorRotation(RedGroup[0]->GetComponentRotation());
				DoAttack(CurSkillID);
			}
			else if (EnemyPawns.Contains(CurAttacker))
			{				
				CurAttacker->SetActorRotation(BlueGroup[0]->GetComponentRotation());
				DoAttack(CurSkillID);
			}			
		}
	}
}

void ABaseBattleLevelActor::MoveBackAfterAttack()
{
	if (CurAttacker && CurSelectEnemy)
	{
		AController* Contr = (CurAttacker->GetController());
		if (Contr)
		{
			if (PlayerPawns.Contains(CurAttacker))
			{
				CurAttackerMoveTarget = RedGroup[PlayerPawns[CurAttacker].LocIndex]->GetComponentLocation();				
			}
			else if(EnemyPawns.Contains(CurAttacker))
			{
				CurAttackerMoveTarget = BlueGroup[EnemyPawns[CurAttacker].LocIndex]->GetComponentLocation();
			}
			CurAttackerMoveTarget.Z = CurAttacker->GetActorLocation().Z;

			UNavigationSystem* const NavSys = GetWorld()->GetNavigationSystem();
			if (NavSys)
			{
				GetWorld()->GetTimerManager().SetTimer(CheckForAttackerMoveTimer, this, &ABaseBattleLevelActor::CheckForAttackerMoveBack, 0.1f, true);

				NavSys->SimpleMoveToLocation(Contr, CurAttackerMoveTarget);
			}
		}
	}	
}

void ABaseBattleLevelActor::CheckForAttackerMoveBack()
{
	if (CurAttacker && CurSelectEnemy)
	{
		if (CurAttacker->GetVelocity().Size() == 0.f/*(CurAttackerMoveTarget - CurAttacker->GetActorLocation()).Size() < 0.5f*/)
		{
			GetWorld()->GetTimerManager().ClearTimer(CheckForAttackerMoveTimer);

			FRotator rot = FRotator(ForceInitToZero);
			FVector loc = FVector(ForceInitToZero);
			if (PlayerPawns.Contains(CurAttacker))
			{
				rot = RedGroup[PlayerPawns[CurAttacker].LocIndex]->GetComponentRotation();
				loc = RedGroup[PlayerPawns[CurAttacker].LocIndex]->GetComponentLocation();
			}
			else if (EnemyPawns.Contains(CurAttacker))
			{
				rot = BlueGroup[EnemyPawns[CurAttacker].LocIndex]->GetComponentRotation();
				loc = BlueGroup[EnemyPawns[CurAttacker].LocIndex]->GetComponentLocation();
			}
			CurAttacker->SetActorRotation(rot);		
			CurAttacker->SetActorLocation(loc);
			HandleAttackEnd();
		}
	}
}

void ABaseBattleLevelActor::DoAttack(int32 SkillID)
{
	if (CurAttacker && CurSelectEnemy)
	{
		float time = CurAttacker->DoAttack(SkillID);
		if (time>0.f)
		{
			GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &ABaseBattleLevelActor::HandleAttackOver, time, false);
		}
		else
		{
			HandleAttackOver();
		}
	}
}

void ABaseBattleLevelActor::HandleAttackOver()
{
	if (CurAttacker && CurSelectEnemy)
	{
		GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
		for (int32 i = 0; i<CurDamageEnemys.Num(); i++)
		{
			CurDamageEnemys[i]->HandleDamage(CurAttacker, CurSkillInfo);
		}

		if (CurSkillInfo.bIsLongDisSkill)
		{
			HandleAttackEnd();
		}
		else
		{
			MoveBackAfterAttack();
		}	
	}
}


void ABaseBattleLevelActor::HandleAttackEnd()
{
	if (CurAttacker && CurAttacker->GetHPWidget())
	{
		CurAttacker->GetHPWidget()->SetArrowVisible(false);
	}
	ClearCurDamageEnemys();
	if (BattleUI)
	{
		BattleUI->OnAttackEnd();
	}
	CurAttacker = nullptr;
	CurSelectEnemy = nullptr;
	CurSkillID = 0;
	bIsInBattleTick = true;

	CurRowOrColumn = 0;
	CurDamageEnemys.Empty();
	CheckForBattleEnd();
}

void ABaseBattleLevelActor::CheckForBattleEnd()
{
	int32 PlayerNum = 0;
	for (auto It = PlayerPawns.CreateConstIterator(); It; ++It)
	{
		if (It.Key()->bIsAlive)
		{
			PlayerNum++;
		}
	}
	if (PlayerNum == 0)
	{
		HandlePlayerLose();
	}
	else
	{
		int32 EnemyNum = 0;
		for (auto It = EnemyPawns.CreateConstIterator(); It; ++It)
		{
			if (It.Key()->bIsAlive)
			{
				EnemyNum++;
			}
		}
		if (EnemyNum == 0)
		{
			HandlePlayerWin();
		}
	}	
}

void ABaseBattleLevelActor::OnDefense()
{
	if (CurAttacker)
	{
		CurAttacker->bIsInDefense = true;
		bIsInBattleTick = true;
		HandleAttackEnd();
	}
}

void ABaseBattleLevelActor::GetEnemyForDiffDamageRange()
{
	if (CurAttacker && CurAttacker->GetSkillInfo(CurSkillID, CurSkillInfo))
	{
		int32 CurSkillDamageRange = CurSkillInfo.DamageRange;
		if (CurSkillDamageRange == 0) //单体
		{
			CurDamageEnemys.Add(CurSelectEnemy);
		}
		else if (CurSkillDamageRange == 1)	//行
		{
			CurRowOrColumn = EnemyPawns[CurSelectEnemy].LocIndex < 3 ? 0 : 1;
			GetEnemyInRow(CurDamageEnemys, CurRowOrColumn);
		}
		else if (CurSkillDamageRange == 2)	//列
		{
			CurRowOrColumn = EnemyPawns[CurSelectEnemy].LocIndex < 3 ? EnemyPawns[CurSelectEnemy].LocIndex : EnemyPawns[CurSelectEnemy].LocIndex - 3;
			GetEnemyInColumn(CurDamageEnemys, CurRowOrColumn);
		}
		else if (CurSkillDamageRange == 3)	//全体
		{
			GetAllEnemy(CurDamageEnemys);
		}
	}
}

void ABaseBattleLevelActor::GetAllEnemy(TArray<ABaseBattlePawn*>& enemys)
{
	for (auto It = EnemyPawns.CreateConstIterator(); It; ++It)
	{
		if (It.Key() && It.Key()->bIsAlive)
		{
			enemys.Add(It.Key());
		}
	}
}

void ABaseBattleLevelActor::GetEnemyInRow(TArray<ABaseBattlePawn*>& enemys, int32 Row)
{
	if (Row == 0)
	{
		for (auto It = EnemyPawns.CreateConstIterator(); It; ++It)
		{
			if (It.Key() && It.Key()->bIsAlive && It.Value().LocIndex < 3)
			{
				enemys.Add(It.Key());
			}
		}
	}
	else if (Row == 1)
	{
		for (auto It = EnemyPawns.CreateConstIterator(); It; ++It)
		{
			if (It.Key() && It.Key()->bIsAlive && It.Value().LocIndex > 2)
			{
				enemys.Add(It.Key());
			}
		}
	}
}

void ABaseBattleLevelActor::GetEnemyInColumn(TArray<ABaseBattlePawn*>& enemys, int32 Column)
{
	if (CurSelectEnemy)
	{
		enemys.Add(CurSelectEnemy);
		if (EnemyPawns.Contains(CurSelectEnemy))
		{
			int32 NextIndex = EnemyPawns[CurSelectEnemy].LocIndex < 3 ? EnemyPawns[CurSelectEnemy].LocIndex + 3 : EnemyPawns[CurSelectEnemy].LocIndex - 3;
			for (auto It = EnemyPawns.CreateConstIterator(); It; ++It)
			{
				if (It.Key() && It.Value().LocIndex == NextIndex && It.Key()->bIsAlive)
				{
					enemys.Add(It.Key());
				}
			}			
		}
	}
}

void ABaseBattleLevelActor::ClearCurDamageEnemys()
{
	if (CurDamageEnemys.Num() > 0)
	{
		for (int32 i = 0; i < CurDamageEnemys.Num(); i++)
		{
			if (CurDamageEnemys[i] && CurDamageEnemys[i]->GetHPWidget())
			{
				CurDamageEnemys[i]->GetHPWidget()->SetSelectedImageVisible(false);
			}
		}
	}
	CurSelectEnemy = nullptr;
	CurDamageEnemys.Empty();
}