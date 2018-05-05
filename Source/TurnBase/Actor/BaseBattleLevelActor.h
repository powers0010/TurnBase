// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Globe/CustomMSG.h"
#include "BaseBattleLevelActor.generated.h"

class UUIBattleWidget;
class ABaseBattlePawn;

UCLASS()
class TURNBASE_API ABaseBattleLevelActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseBattleLevelActor();

	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return BattleLevelCamera; }

	FORCEINLINE	void SetIsInEnemySelect(bool InBool) { bIsInEnemySelect = InBool; }

	FORCEINLINE ABaseBattlePawn* GetCurAttacker() { return CurAttacker; }

	FORCEINLINE UUIBattleWidget* GetBattleUI() { return BattleUI; }
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* BattleLevelCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* LevelMesh;

	UPROPERTY(EditAnywhere)
		TArray<FVector> RedArrowLoc;

	UPROPERTY(EditAnywhere)
		TArray<FVector> BlueArrowLoc;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Arrow, meta = (AllowPrivateAccess = "true"))
		TArray<class UArrowComponent*> RedGroup;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Arrow, meta = (AllowPrivateAccess = "true"))
		TArray<class UArrowComponent*> BlueGroup;

	UPROPERTY()
		UUIBattleWidget* BattleUI;

	class ATurnBaseCharacter* Player = nullptr;

	class ABaseEnemy* Enemy = nullptr;

	UPROPERTY()
		TMap<ABaseBattlePawn*, FPawnFightMsg> PlayerPawns;

	UPROPERTY()
		TMap<ABaseBattlePawn*, FPawnFightMsg> EnemyPawns;

	bool bIsInBattleTick = false;

	bool bIsInEnemySelect = false;

	ABaseBattlePawn* CurAttacker = nullptr;

	FVector CurAttackerMoveTarget;

	ABaseBattlePawn* CurSelectEnemy = nullptr;

	TArray<ABaseBattlePawn*> CurDamageEnemys;
	FSkillInfo CurSkillInfo;
	int32 CurRowOrColumn = 0;

public:
	int32 CurSkillID = 0;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void BattleTick(float DeltaTime);

	UPROPERTY(EditAnywhere)
		EBattleLevelType LevelType;

	void BattleStart(class ATurnBaseCharacter* player, class ABaseEnemy* enemy);

	void BattleEnd();

	void PlayerDoAttack();

	void HandleAttackEnd();

	void OnDefense();

	void ClearCurDamageEnemys();

	void OnMoveSuccess(bool bIsToAttacking);
protected:
	void PlayerSelectEnemy(ABaseBattlePawn* Attacker);
	
	void AISelectEnemy(ABaseBattlePawn* Attacker);

	void DoEnemySelect();

	void MoveToAttackerTargetLoc();

	void MoveBackAfterAttack();

	void CheckForAttackerMoveToTargetLoc();
	FTimerHandle CheckForAttackerMoveTimer;

	void CheckForAttackerMoveBack();

	void DoAttack(int32 SkillID);
	FTimerHandle AttackTimerHandle;

	void HandleAttackOver();

	void CheckForBattleEnd();

	void HandlePlayerWin();

	void HandlePlayerLose();

	void GetEnemyForDiffDamageRange();

	void GetAllEnemy(TArray<ABaseBattlePawn*>& enemys);

	void GetEnemyInRow(TArray<ABaseBattlePawn*>& enemys, int32 Row);

	void GetEnemyInColumn(TArray<ABaseBattlePawn*>& enemys, int32 Column);

};
