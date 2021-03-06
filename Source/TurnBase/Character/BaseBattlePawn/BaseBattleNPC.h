// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseBattlePawn/BaseBattlePawn.h"
#include "BaseBattleNPC.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASE_API ABaseBattleNPC : public ABaseBattlePawn
{
	GENERATED_BODY()
	
public:
	virtual	void InitPawnFromData(int32 id) override;

	FORCEINLINE float GetValueEXP() { return ValueEXP; }

protected:
	float ValueEXP;

	
	
};
