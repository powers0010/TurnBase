// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
class UDataAssetManager;

UCLASS()
class TURNBASE_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	
	FORCEINLINE UDataAssetManager* GetDataManager() {	return DataManager;	}

public:
	UPROPERTY()
		UDataAssetManager* DataManager = nullptr;
};
