// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_DataListEntryMapping.generated.h"


class UListDataObject_Base;	

UCLASS()
class PROJECT_IX_API UDataAsset_DataListEntryMapping : public UDataAsset
{
	GENERATED_BODY()
	
private:

	UPROPERTY(EditDefaultsOnly, Category = "DataListEntryMapping")
	TMAP<TSubclassOf<UListDataObject_Base>, TSubclassOf
};
