// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIFightSeqWidget.generated.h"

/**
 * 
 */
class UImage;
class UTexture2D;

UCLASS()
class TURNBASE_API UUIFightSeqWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	void SetPawnIcon(UTexture2D* image);
protected:
	UImage * PawnIcon = nullptr;
	
	
};
