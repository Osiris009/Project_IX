// Fill out your copyright notice in the Description page of Project Settings.


#include "IX_UI/Widgets/OptionScreens/OptionsDataRegistry.h"

#include "IX_UI/Widgets/OptionScreens/DataObjects/ListDataObject_Collection.h"
#include "IX_UI/Widgets/OptionScreens/DataObjects/ListDataObject_String.h"

void UOptionsDataRegistry::InitOptionsDataRegistry(ULocalPlayer* InOwningLocalPlayer)
{
	InitGameplayCollectionTab();
	InitAudioCollectionTab();
	InitVideoCollectionTab();	
	InitControlsCollectionTab();
}

TArray<UListDataObject_Base*> UOptionsDataRegistry::GetListSourceItemsBySelectedTabID(const FName& InSelectedTabID) const
{
	UListDataObject_Collection* const* FoundCollectionPtr = RegisteredOptionsTabCollections.FindByPredicate(
		[InSelectedTabID](UListDataObject_Collection* AvailableTabCollection)->bool
		{
			return AvailableTabCollection->GetDataID() == InSelectedTabID;
		}
	);
	checkf(FoundCollectionPtr, TEXT("No collection found for the given tab ID: %s"), *InSelectedTabID.ToString());

	UListDataObject_Collection* FoundTabCollection = *FoundCollectionPtr;
	return FoundTabCollection->GetAllChildListData();// Return the child list data of the found collection, 
	//which will be used as the source items for the options list view
}

void UOptionsDataRegistry::InitGameplayCollectionTab()
{
	UListDataObject_Collection* GameplayCollectionTab = NewObject<UListDataObject_Collection>();
	
	GameplayCollectionTab->SetDataID(FName("GameplayCollectionTab"));
	
	GameplayCollectionTab->SetDataDisplayName(FText::FromString("Gameplay"));

	//GameDifficultyOption
	{
		
		UListDataObject_String* GameDifficulty = NewObject<UListDataObject_String>();
		GameDifficulty->SetDataID(FName("GameDifficulty"));
		GameDifficulty->SetDataDisplayName(FText::FromString("Difficulty"));
		GameplayCollectionTab->AddChildListData(GameDifficulty);
	}

	////TestingOption
	//{
	//	UListDataObject_String* TestingItem = NewObject<UListDataObject_String>();
	//	TestingItem->SetDataID(FName("Testing"));
	//	TestingItem->SetDataDisplayName(FText::FromString("Testing "));
	//	GameplayCollectionTab->AddChildListData(TestingItem);
	//}

	RegisteredOptionsTabCollections.Add(GameplayCollectionTab);
}

void UOptionsDataRegistry::InitAudioCollectionTab()
{
	UListDataObject_Collection* AudioCollectionTab = NewObject<UListDataObject_Collection>();

	AudioCollectionTab->SetDataID(FName("AudioCollectionTab"));

	AudioCollectionTab->SetDataDisplayName(FText::FromString("Audio"));	
	RegisteredOptionsTabCollections.Add(AudioCollectionTab);
}

void UOptionsDataRegistry::InitVideoCollectionTab()
{
	UListDataObject_Collection* VideoCollectionTab = NewObject<UListDataObject_Collection>();
	VideoCollectionTab->SetDataID(FName("VideoCollectionTab"));
	VideoCollectionTab->SetDataDisplayName(FText::FromString("Video"));	
	RegisteredOptionsTabCollections.Add(VideoCollectionTab);
}

void UOptionsDataRegistry::InitControlsCollectionTab()
{
	UListDataObject_Collection* ControlsCollectionTab = NewObject<UListDataObject_Collection>();
	ControlsCollectionTab->SetDataID(FName("ControlsCollectionTab"));
	ControlsCollectionTab->SetDataDisplayName(FText::FromString("Controls"));	
	RegisteredOptionsTabCollections.Add(ControlsCollectionTab);
}
