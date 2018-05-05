// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Globe/CustomMSG.h"
#include "DataAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASE_API UDataAssetManager : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DataAsset")
		TAssetPtr<class UDataTable>   HeroAttributeMsg;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DataAsset")
		TAssetPtr<class UDataTable>   SkillData;
public:
	FAttributeData* GetAttributeAsset( int32 key);

	FSkillInfo* GetSkillData(int32 key);
};
