// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIBattleWidget.generated.h"

/**
 * 
 */
class UButton;
class UVerticalBox;
class UCanvasPanel;
class UImage;
class ABaseBattlePawn;
class ABaseBattleLevelActor;


UCLASS()
class TURNBASE_API UUIBattleWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	FORCEINLINE void InitLevelActor(ABaseBattleLevelActor* CurLevelActor) { LecelActor = CurLevelActor; }

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	UPROPERTY()
		TSubclassOf<class UUIFightSeqWidget> FightSeqClass = nullptr;
	UPROPERTY()
		TSubclassOf<class UUISkillButton> SkillBtnClass = nullptr;
	bool bNeedInitSkillBorder = true;

	UButton * AtkBtn = nullptr;

	UButton* SkillBtn = nullptr;

	UButton* DefenseBtn = nullptr;

	UButton* WinBtn = nullptr;

	UButton* LoseBtn = nullptr;

	UVerticalBox* AttackOrderPanel = nullptr;

	UVerticalBox* SkillOrderPanel = nullptr;

	UCanvasPanel* FightSeqCanvas = nullptr;

	ABaseBattleLevelActor* LecelActor = nullptr;

	UPROPERTY()
		TMap <ABaseBattlePawn*, UUserWidget* > FightSeqImages;

protected:
	UFUNCTION()
		void OnAttackBthClick();
	UFUNCTION()
		void OnDefenseBtnClick();
	UFUNCTION()
		void OnWinBthClick();
	UFUNCTION()
		void OnSkillBtnClick();
public:
	void OnBattleEnd();

	void OnAttackEnd();

	void SetAttackOrderVisible(bool NewVis);

	void SetSkillOrderVisible(bool NewVis);

	void SetWinBorderVisible(bool NewVis);

	void SetLoseBorderVisible(bool NewVis);

	void AddFightSeqImage(class ABaseBattlePawn* OnPawn);

	void SetFightSeqImageOffset(ABaseBattlePawn* bpawn,float SeqPercent);

	void SetFightSeqImageVisible(ABaseBattlePawn* bpawn, bool NewVis);

	void ClearSkillBorder();
};
