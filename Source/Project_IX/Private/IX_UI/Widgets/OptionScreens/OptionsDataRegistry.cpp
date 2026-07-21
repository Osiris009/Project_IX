// Fill out your copyright notice in the Description page of Project Settings.


#include "IX_UI/Widgets/OptionScreens/OptionsDataRegistry.h"

#include "IX_UI/Widgets/OptionScreens/DataObjects/ListDataObject_Collection.h"
#include "IX_UI/Widgets/OptionScreens/DataObjects/ListDataObject_String.h"

#include "IX_UI/Widgets/OptionScreens/OptionDataInterationHelper.h"
#include "IXProjectSettings/PIXGameUserSettings.h"
#include "IX_UI/Extra/UIGamePlayTags.h"
#include "IXFuctionLibrary/UIFunctionLibrary.h"

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
		GameDifficulty->SetDescriptionRichText(FText::FromString(TEXT("Adjusts the difficulty of the game experience.\n\n<Bold>Easy:</> Focuses on the story experience. Provides the most relaxing combat.\n\n<Bold>Normal:</> Offers slightly harder combat experience\n\n<Bold>Hard:</>Offers a much more challenging combat experience\n\n<Bold>Vert Hard:</> Provides the most challenging combat experience. Not recommended for first play through.")));
		GameDifficulty->AddDynamicOption(TEXT("Easy"), FText::FromString(TEXT("Easy")));
		GameDifficulty->AddDynamicOption(TEXT("Normal"), FText::FromString(TEXT("Normal")));
		GameDifficulty->AddDynamicOption(TEXT("Hard"), FText::FromString(TEXT("Hard")));
		GameDifficulty->AddDynamicOption(TEXT("VeryHard"), FText::FromString(TEXT("VeryHard")));

		GameDifficulty->SetDefaultValueFromString(TEXT("Normal")); // Set the default value for the GameDifficulty option


		// Set the dynamic getter and setter for the GameDifficulty option using the macro
		GameDifficulty->SetDtataDynamicGetter(MAKE_OPTION_DATA_CONTROL(GetCurrentGameDifficulty));
		GameDifficulty->SetDtataDynamicSetter(MAKE_OPTION_DATA_CONTROL(SetCurrentGameDifficulty)); 
		// Add the GameDifficulty option to the GameplayCollectionTab

		GameDifficulty->SetShouldApplyChangeImimediately(true); // Set the option to apply changes immediately when modified	

		GameplayCollectionTab->AddChildListData(GameDifficulty);
		
	}

	////TestingOption
	{
		UListDataObject_String* TestingItem = NewObject<UListDataObject_String>();
		TestingItem->SetDataID(FName("Testing"));
		TestingItem->SetDataDisplayName(FText::FromString(TEXT("Test Image Item")));
		TestingItem->SetSoftDescriptionImage(UUIFunctionLibrary::GetOptionsSoftImageByTag(IXGameplayTags::IXUI_Image_TestImage));
		TestingItem->SetDescriptionRichText(FText::FromString(TEXT("The image to display can be specified in the project settings. It can be anything the developer assigned in there")));
		GameplayCollectionTab->AddChildListData(TestingItem);
	}

	RegisteredOptionsTabCollections.Add(GameplayCollectionTab);
}

void UOptionsDataRegistry::InitAudioCollectionTab()
{
	UListDataObject_Collection* AudioCollectionTab = NewObject<UListDataObject_Collection>();

	AudioCollectionTab->SetDataID(FName("AudioCollectionTab")); //from which the options screen will know which tab is selected

	AudioCollectionTab->SetDataDisplayName(FText::FromString("Audio"));	// The display name of the tab that will be shown in the options screen

	//Volume Catagory 

	{
		UListDataObject_Base* VolumeCategoryCollection = NewObject<UListDataObject_Collection>();
		VolumeCategoryCollection->SetDataID(FName("VolumeCategoryCollection"));
		VolumeCategoryCollection->SetDataDisplayName(FText::FromString("Volume")); // The display name of the category that will be shown in the options screen

		AudioCollectionTab->AddChildListData(VolumeCategoryCollection);
	}

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
