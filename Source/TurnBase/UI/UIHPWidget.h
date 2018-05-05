// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIHPWidget.generated.h"

/**
 * 
 */
class UImage;
class UTextBlock;

UCLASS()
class TURNBASE_API UUIHPWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	class ABaseBattlePawn* owner = nullptr;
protected:
	UTextBlock * NameTxt = nullptr;

	UTextBlock* HPTxt = nullptr;

	UImage* ArrowImage = nullptr;
	
	UImage* SelectImage = nullptr;

public:
	void SetNameVisible(bool NewVis);
	void SetHPVisible(bool NewVis);
	void SetArrowVisible(bool NewVis);
	void SetSelectedImageVisible(bool NewVis);

	void SetHPTxt(float HP, float MaxHP);
	void SetNameTxt(FText name);
};
