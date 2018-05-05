// Fill out your copyright notice in the Description page of Project Settings.

#include "DataAssetManager.h"
#include "Classes/Engine/StreamableManager.h"



FHeroAttributeData * UDataAssetManager::GetHeroAttributeAsset(int32 key)
{
	FStreamableManager mAssetLoader;
	UObject* obj = mAssetLoader.LoadSynchronous(HeroAttributeMsg.ToSoftObjectPath());
	UDataTable* data = Cast<UDataTable>(obj);
	if (data != nullptr)
	{
		FString keyname = FString::Printf(TEXT("%d"), key);
		if (data->RowMap.Contains(*keyname))
		{
			FHeroAttributeData* msg = (FHeroAttributeData*)(data->RowMap[*keyname]);
			return msg;
		}
	}
	return nullptr;
}

FPawnAttributeData* UDataAssetManager::GetPawnAttributeAsset(int32 key)
{
	FStreamableManager mAssetLoader;
	UObject* obj = mAssetLoader.LoadSynchronous(PawnAttributeMsg.ToSoftObjectPath());
	UDataTable* data = Cast<UDataTable>(obj);
	if (data != nullptr)
	{
		FString keyname = FString::Printf(TEXT("%d"), key);
		if (data->RowMap.Contains(*keyname))
		{
			FPawnAttributeData* msg = (FPawnAttributeData*)(data->RowMap[*keyname]);
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