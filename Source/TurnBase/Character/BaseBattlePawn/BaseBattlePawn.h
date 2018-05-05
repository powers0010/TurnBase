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

	FORCEINLINE class UUIHPWidget* GetHPWidget() { return HPWidget; }

	FORCEINLINE void InitAttribute(FAttribute& InAttribute) { Attribute = InAttribute; }

	FORCEINLINE FAttribute& GetAttribute() { return Attribute; }

	FORCEINLINE TMap<int32, FSkillInfo>& GetSkillInfos() { return SkillInfos; }

	bool GetSkillInfo(int32 SkillID, FSkillInfo& info);

	void InitPawnFromData(int32 id);

	
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

public:
	UPROPERTY(EditDefaultsOnly)
		int32 AttributeID = 0;
	UPROPERTY(EditDefaultsOnly)
		FAttribute Attribute;
	
	bool bIsAlive = true;

	bool bIsInDefense = false;

	class ABaseBattleLevelActor* CurLevelActor = nullptr;
public:

	float DoAttack(int32 SkillID);

	void HandleDamage(ABaseBattlePawn* Attacker,FSkillInfo& skillinfo);

	void Die();

	int32 AISelectSkill();

	void OnWin();

};
