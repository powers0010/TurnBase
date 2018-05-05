// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UISkillButton.generated.h"

/**
 * 
 */
class UButton;
class UTextBlock;
class ABaseBattleLevelActor;


UCLASS()
class TURNBASE_API UUISkillButton : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
	void SetSkillName(FText name);

	int32 SkillId = 1;

	ABaseBattleLevelActor* LecelActor = nullptr;
protected:
	UButton * SkillBtn = nullptr;

	UTextBlock * SkillTxt = nullptr;

	UFUNCTION()
		void OnSkillbtnClick();
};
