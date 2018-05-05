// Fill out your copyright notice in the Description page of Project Settings.

#include "DataAssetManager.h"
#include "Classes/Engine/StreamableManager.h"



FAttributeData* UDataAssetManager::GetAttributeAsset(int32 key)
{
	FStreamableManager mAssetLoader;
	UObject* obj = mAssetLoader.LoadSynchronous(HeroAttributeMsg.ToSoftObjectPath());
	UDataTable* data = Cast<UDataTable>(obj);
	if (data != nullptr)
	{
		FString keyname = FString::Printf(TEXT("%d"), key);
		if (data->RowMap.Contains(*keyname))
		{
			FAttributeData* msg = (FAttributeData*)(data->RowMap[*keyname]);
			return msg;
		}
	}
	return nullptr;
}

FSkillInfo* UDataAssetManager::GetSkillData(int32 key)
{
	FStreamableManager mAssetLoader;
	UObject* obj = mAssetLoader.LoadSynchronous(SkillData.ToSoftObjectPath());
	UDataTable* data = Cast<UDataTable>(obj);
	if (data != nullptr)
	{
		FString keyname = FString::Printf(TEXT("%d"), key);
		if (data->RowMap.Contains(*keyname))
		{
			FSkillInfo* msg = (FSkillInfo*)(data->RowMap[*keyname]);
			return msg;				
		}
	}
	return nullptr;
}