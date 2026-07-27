// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "OptionsDataRegistry.generated.h"


class UListDataObject_Collection;
class UListDataObject_Base;

UCLASS()
class PROJECT_IX_API UOptionsDataRegistry : public UObject
{
	GENERATED_BODY()

public:

	//Gets called by Options screen right after the UOptionsDataRegistry is created.
	// The registry will then create all the data objects and store them for the options screen to use
	void InitOptionsDataRegistry(ULocalPlayer* InOwningLocalPlayer);

	const TArray<UListDataObject_Collection*>& GetRegisteredOptionsTabCollections() const { return RegisteredOptionsTabCollections; }

	TArray<UListDataObject_Base*> GetListSourceItemsBySelectedTabID(const FName& InSelectedTabID) const;


private:
	
	void FindChildListDataRecursively(UListDataObject_Base* InParentData, TArray<UListDataObject_Base*>& OutFoundChildListData) const;

	void InitGameplayCollectionTab();       

	void InitAudioCollectionTab();
	
	void InitVideoCollectionTab();
	
	void InitControlsCollectionTab();

	UPROPERTY(Transient)
	TArray<UListDataObject_Collection*> RegisteredOptionsTabCollections;



};
