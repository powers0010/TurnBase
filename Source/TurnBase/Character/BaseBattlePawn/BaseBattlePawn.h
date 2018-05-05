// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Globe/CustomMSG.h"
#include "BaseBattlePawn.generated.h"


UCLASS()
class TURNBASE_API ABaseBattlePawn : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseBattlePawn();

	virtual void Tick(float DeltaTime) override;

	virtual	void InitPawnFromData(int32 id) {}

	bool GetSkillInfo(int32 SkillID, FSkillInfo& info);
	
	FORCEINLINE class UUIHPWidget* GetHPWidget() { return HPWidget; }
	FORCEINLINE TMap<int32, FSkillInfo>& GetSkillInfos() { return SkillInfos; }
	FORCEINLINE FText GetViewName() { return FText::FromString(FString::Printf(TEXT("%s LV%d"), *PawnName.ToString(), CurLevel))  ; }
	FORCEINLINE float GetMaxHP() { return MaxHP; }
	FORCEINLINE float GetCurHP() { return CurHP; }
	FORCEINLINE float GetCurBattleRateSpeed() { return CurBattleRateSpeed; }
	FORCEINLINE float GetCurATK() { return CurATK; }
	FORCEINLINE float GetCurMagicATK() { return CurMagicATK; }
	FORCEINLINE float GetCurDefence() { return CurDefense; }
	FORCEINLINE float GetCurMagicDefence() { return CurMagicDefense; }
	FORCEINLINE UTexture2D* GetFightSeqImage() { return FightSeqImage; }

	FORCEINLINE void SetCurHP(float newhp) { CurHP = newhp; }
	FORCEINLINE void DecCurHP(float damage) { CurHP -= damage; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, meta = (AllowPrivateAccess = "true"))
		class UWidgetComponent* WidgetComComponent;

	class UUIHPWidget* HPWidget = nullptr;

	UPROPERTY(EditDefaultsOnly)
		TMap<int32, FSkillInfo> SkillInfos;

	UPROPERTY(EditDefaultsOnly)
		class UAnimSequence* AnimSeq_Die = nullptr;
	UPROPERTY(EditDefaultsOnly)
		class UAnimSequence* AnimSeq_Win = nullptr;
	UPROPERTY(EditDefaultsOnly)
		class UAnimMontage* AnimMon_GetHit = nullptr;
	UPROPERTY(EditDefaultsOnly)
		class UAnimMontage* AnimMon_Attack = nullptr;

	int32 AttributeID = 0;
	//----------Attribute-------------------
	FText PawnName;
	float MaxHP;
	float CurHP;
	float CurATK;
	float CurMagicATK;
	float CurDefense;
	float CurMagicDefense;
	float CurBattleRateSpeed;
	class UTexture2D* FightSeqImage = nullptr;
	int32 CurLevel;

public:	
	bool bIsAlive = true;

	bool bIsInDefense = false;

	class ABaseBattleLevelActor* CurLevelActor = nullptr;


public:
	float DoAttack(int32 SkillID);

	void HandleDamage(ABaseBattlePawn* Attacker,FSkillInfo& skillinfo);

	void Die();

	int32 AISelectSkill();

	virtual void OnWin( float GainExp);

//----------------------战斗Level中的移动与旋转处理---------------------
public:
	void MoveToTarget(FVector& TargetLoc, bool bIsToAttacking);
	void RotatorToTargetRotator(FRotator& Rot);

	FVector MoveOrign;
	FVector MoveTargetLoc;
	bool bNeedMove = false;
	bool bIsMoveToAttacking = true;

	bool bNeedRot = false;
	FRotator TargetRot;
	float DeYaw;

};
