// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Classes/Engine/DataTable.h"
#include "CustomMSG.generated.h"

/**
 * 
 */


const float MaxWalkSpeed = 400.f;
const FVector2D CapsuleSize = FVector2D(30.f, 60.f);
const FVector MeshScale = FVector(1.5f);



//---------------≈‰±Ì-----------

USTRUCT(BlueprintType)
struct FPawnAttributeData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FAttributeData")
		int32 Id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FAttributeData")
		FText PawnName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FAttributeData")
		FString MeshPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FAttributeData")
		FString AnimBPPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FAttributeData")
		FString AnimSeq_Die;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FAttributeData")
		FString AnimMon_Attack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FAttributeData")
		FString AnimMon_GetHit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FAttributeData")
		FString Image_FightSeq;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FAttributeData")
		FString AnimSeq_Win;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FAttributeData")
		float MaxHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FAttributeData")
		float ATK;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FAttributeData")
		float MagicATK;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FAttributeData")
		float Defense;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FAttributeData")
		float MagicDefense;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FAttributeData")
		float BattleRateSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FAttributeData")
		int32 SkillID;
};

USTRUCT(BlueprintType)
struct FPawnAttribute
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
		FText PawnName;
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
		float MaxHP;
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
		float CurHP;

	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
		float ATK;
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
		float CurATK;
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
		float MagicATK;
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
		float CurMagicATK;

	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
		float Defense;
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
		float CurDefense;
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
		float MagicDefense;
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
		float CurMagicDefense;

	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
		float BattleRateSpeed;
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
		float CurBattleRateSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
		int32 CurLevel;

	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
		class UTexture2D* FightSeqImage;

	FPawnAttribute()
	{}
};

USTRUCT(BlueprintType)
struct FHeroAttributeData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FAttributeData")
		int32 Id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FAttributeData")
		FText PawnName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FAttributeData")
		FString MeshPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FAttributeData")
		FString AnimBPPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FAttributeData")
		FString AnimSeq_Die;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FAttributeData")
		FString AnimMon_Attack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FAttributeData")
		FString AnimMon_GetHit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FAttributeData")
		FString Image_FightSeq;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FAttributeData")
		FString AnimSeq_Win;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FAttributeData")
		float MaxHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FAttributeData")
		float ATK;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FAttributeData")
		float MagicATK;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FAttributeData")
		float Defense;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FAttributeData")
		float MagicDefense;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FAttributeData")
		float BattleRateSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FAttributeData")
		int32 SkillID;
};

USTRUCT(BlueprintType)
struct FHeroAttribute
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(BlueprintReadOnly, Category = "Attribute")
		FText PawnName;
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
		float MaxHP;
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
		float CurHP;

	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
		float ATK;
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
		float CurATK;
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
		float MagicATK;
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
		float CurMagicATK;

	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
		float Defense;
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
		float CurDefense;
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
		float MagicDefense;
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
		float CurMagicDefense;

	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
		float BattleRateSpeed;
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
		float CurBattleRateSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
		int32 CurLevel;

	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
		class UTexture2D* FightSeqImage;

	FHeroAttribute()
	{}
};

USTRUCT(BlueprintType)
struct FSkillInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillInfo")
		int32 Id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillInfo")
		FString SkillName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillInfo")
		FText ViewName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FSkillData")
		FString AnimPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillInfo")
		class UAnimMontage* AnimMon_Skill;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillInfo")
		int32 DamageType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillInfo")
		float Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillInfo")
		int32 DamageRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillInfo")
		bool bIsLongDisSkill;

	FSkillInfo(){}
	FSkillInfo(class UAnimMontage* Anim, float da) :AnimMon_Skill(Anim), Damage(da), DamageRange(0), bIsLongDisSkill(false), DamageType(0)
	{}
	FSkillInfo(FSkillInfo* infoptr)
	{
		*this = *infoptr;
	}
};


UENUM(BlueprintType)
enum class EBattleLevelType : uint8
{
	EBT_BiWuTai  UMETA(DisplayName = "±»Œ‰Ã®")
};

USTRUCT(BlueprintType)
struct FPawnFightMsg
{
	GENERATED_USTRUCT_BODY()
		UPROPERTY(BlueprintReadOnly, Category = "Attribute")
		float FightSeq;
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
		int32 LocIndex;

	FPawnFightMsg() :FightSeq(0.f), LocIndex(0)
	{}
	FPawnFightMsg(int32 LIdex) :LocIndex(LIdex), FightSeq(0.f)
	{}
};


//
//
//class TURNBASE_API CustomMSG
//{
//public:
//	CustomMSG();
//	~CustomMSG();
//};
