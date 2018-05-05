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
	
	
};
