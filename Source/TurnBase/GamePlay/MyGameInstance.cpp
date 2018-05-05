// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameInstance.h"
#include "Globe/DataAssetManager.h"

void UMyGameInstance::Init()
{
	Super::Init();

	if (!DataManager)
	{
		UClass* DataManagerClass = LoadClass<UDataAssetManager>(NULL, TEXT("/Game/Data/DataAssetManagerBP.DataAssetManagerBP_C"));
		if(DataManagerClass)
		{
			DataManager = NewObject<UDataAssetManager>(this, DataManagerClass);
		}
	}
}


