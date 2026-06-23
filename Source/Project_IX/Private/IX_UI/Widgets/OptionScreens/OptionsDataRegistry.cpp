// Fill out your copyright notice in the Description page of Project Settings.


#include "IX_UI/Widgets/OptionScreens/OptionsDataRegistry.h"

#include "IX_UI/Widgets/OptionScreens/DataObjects/ListDataObject_Collection.h"
#include "IX_UI/Widgets/OptionScreens/DataObjects/ListDataObject_String.h"

#include "IX_UI/Widgets/OptionScreens/OptionDataInterationHelper.h"
#include "IXProjectSettings/PIXGameUserSettings.h"

#define MAKE_OPTION_DATA_CONTROL(SetterOrGetterFuncName) \
	MakeShared<FOptionDataInterationHelper>(GET_FUNCTION_NAME_STRING_CHECKED(UPIXGameUserSettings, SetterOrGetterFuncName))

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
	
	//This is For Constructor Data Interaction Helper, which will be used to interact with the game user settings
	/*TSharedPtr<FOptionDataInterationHelper> ConstructedHelper =
		MakeShared<FOptionDataInterationHelper>(
			GET_FUNCTION_NAME_STRING_CHECKED(UPIXGameUserSettings, GetCurrentGameDifficulty)
		);*/	//*** ***  This macro checks if the function name is valid at compile time, and returns the function name as a string.
	


	//GameDifficultyOption
	{
		
		UListDataObject_String* GameDifficulty = NewObject<UListDataObject_String>();
		GameDifficulty->SetDataID(FName("GameDifficulty"));
		GameDifficulty->SetDataDisplayName(FText::FromString(TEXT("Difficulty")));  
		GameDifficulty->AddDynamicOption(TEXT("Easy"), FText::FromString(TEXT("Easy")));
		GameDifficulty->AddDynamicOption(TEXT("Normal"), FText::FromString(TEXT("Normal")));
		GameDifficulty->AddDynamicOption(TEXT("Hard"), FText::FromString(TEXT("Hard")));
		GameDifficulty->AddDynamicOption(TEXT("VeryHard"), FText::FromString(TEXT("VeryHard")));
		// Set the dynamic getter and setter for the GameDifficulty option using the macro
		GameDifficulty->SetDtataDynamicGetter(MAKE_OPTION_DATA_CONTROL(GetCurrentGameDifficulty));
		GameDifficulty->SetDtataDynamicSetter(MAKE_OPTION_DATA_CONTROL(SetCurrentGameDifficulty)); 
		// Add the GameDifficulty option to the GameplayCollectionTab
		GameplayCollectionTab->AddChildListData(GameDifficulty);
		
	}

	////TestingOption
	{
		UListDataObject_String* TestingItem = NewObject<UListDataObject_String>();
		TestingItem->SetDataID(FName("Testing"));
		TestingItem->SetDataDisplayName(FText::FromString(TEXT( "Testing ")));
		GameplayCollectionTab->AddChildListData(TestingItem);
	}

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
