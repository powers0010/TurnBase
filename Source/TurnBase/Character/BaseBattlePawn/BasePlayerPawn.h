// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseBattlePawn/BaseBattlePawn.h"
#include "BasePlayerPawn.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASE_API ABasePlayerPawn : public ABaseBattlePawn
{
	GENERATED_BODY()
	
public:
	virtual	void InitPawnFromData(int32 id) override;

	void ResetForBattleEnd(FVector loc);
	
	virtual void OnWin(float GainExp) override;

	void GainEXP(float InExp);

	void OnLevelUp(float OverExp);


protected:
	//属性
	float ATK;
	float MagicATK;
	float Defense;
	float MagicDefense;
	float BattleRateSpeed;
	
	float CurEXP = 0.f;
	float CurLevelMaxEXP = 100.f;

	//属性成长
	float ATKGrow;
	float MagicATKGrow;
	float DefenseGrow;
	float MagicDefenseGrow;
	float BattleRateSpeedGrow;
};

namespace LevelEXP
{
	static float CaculatLevelEXP(int32 level)
	{
		return level * 100.f;
	}
}