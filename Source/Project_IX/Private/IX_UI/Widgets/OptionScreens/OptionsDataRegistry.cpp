// Fill out your copyright notice in the Description page of Project Settings.


#include "IX_UI/Widgets/OptionScreens/OptionsDataRegistry.h"

#include "IX_UI/Widgets/OptionScreens/DataObjects/ListDataObject_Collection.h"
#include "IX_UI/Widgets/OptionScreens/DataObjects/ListDataObject_String.h"

#include "IX_UI/Widgets/OptionScreens/OptionDataInterationHelper.h"
#include "IXProjectSettings/PIXGameUserSettings.h"
#include "IX_UI/Extra/UIGamePlayTags.h"
#include "IXFuctionLibrary/UIFunctionLibrary.h"
#include "IX_UI/Widgets/OptionScreens/DataObjects/ListDataObject_Scalar.h"





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
	UListDataObject_Collection* const* FoundTabCollectionPtr = RegisteredOptionsTabCollections.FindByPredicate(
		[InSelectedTabID](UListDataObject_Collection* AvailableTabCollection)->bool
		{
			return AvailableTabCollection->GetDataID() == InSelectedTabID;
		}
	);
	checkf(FoundTabCollectionPtr,TEXT("No valid tab found under the ID %s"),*InSelectedTabID.ToString());

	UListDataObject_Collection* FoundTabCollection = *FoundTabCollectionPtr;

	TArray<UListDataObject_Base*> AllChildListItems;

	for (UListDataObject_Base* ChildListData : FoundTabCollection->GetAllChildListData())
	{
		if (!ChildListData)
		{
			continue;
		}

		AllChildListItems.Add(ChildListData);

		if (ChildListData->HasAnyChildListData())
		{
			FindChildListDataRecursively(ChildListData,AllChildListItems);
		}
	}

	return AllChildListItems;	
	// Return the child list data of the found collection, 
	//which will be used as the source items for the options list view
}

void UOptionsDataRegistry::FindChildListDataRecursively(UListDataObject_Base* InParentData, TArray<UListDataObject_Base*>& OutFoundChildListData) const
{
	if (!InParentData || !InParentData->HasAnyChildListData())
	{
		return;
	}

	for (UListDataObject_Base* SubChildListData : InParentData->GetAllChildListData())
	{
		if (!SubChildListData)
		{
			continue;
		}

		OutFoundChildListData.Add(SubChildListData);

		if (SubChildListData->HasAnyChildListData())
		{
			FindChildListDataRecursively(SubChildListData,OutFoundChildListData);
		}
	}
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
		GameDifficulty->SetDataDynamicGetter(MAKE_OPTION_DATA_CONTROL(GetCurrentGameDifficulty));
		GameDifficulty->SetDataDynamicSetter(MAKE_OPTION_DATA_CONTROL(SetCurrentGameDifficulty)); 
		// Add the GameDifficulty option to the GameplayCollectionTab

		GameDifficulty->SetShouldApplyChangeImimediately(true); // Set the option to apply changes immediately when modified	

		GameplayCollectionTab->AddChildListData(GameDifficulty);
		
	}
	
	//TestingOption
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
	UListDataObject_Collection* AudioTabCollection = NewObject<UListDataObject_Collection>();
	AudioTabCollection->SetDataID(FName("AudioTabCollection"));
	AudioTabCollection->SetDataDisplayName(FText::FromString(TEXT("Audio")));
	
	// The display name of the tab that will be shown in the options screen

	//from which the options screen will know which tab is selected
	//Volume Catagory 

	//Volume Category
	{
		UListDataObject_Collection* VolumeCategoryCollection = NewObject<UListDataObject_Collection>();
		VolumeCategoryCollection->SetDataID(FName("VolumeCategoryCollection"));
		VolumeCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("Volume")));

		AudioTabCollection->AddChildListData(VolumeCategoryCollection);

		//OverAll Volume
		{
			UListDataObject_Scalar* OverallVolume = NewObject<UListDataObject_Scalar>();
			OverallVolume->SetDataID(FName("OverallVolume"));
			OverallVolume->SetDataDisplayName(FText::FromString(TEXT("Overall Volume")));
			OverallVolume->SetDescriptionRichText(FText::FromString(TEXT("This is description for Overall Volume")));
			OverallVolume->SetDisplayValueRange(TRange<float>(0.f,1.f));
			OverallVolume->SetOutputValueRange(TRange<float>(0.f,2.f));
			OverallVolume->SetSliderStepSize(0.01f);
			OverallVolume->SetDefaultValueFromString(LexToString(1.f));
			OverallVolume->SetDisplayNumericType(ECommonNumericType::Percentage);
			OverallVolume->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());  //No Decimal: 50%  //One Decimal: 50.5%
			
			OverallVolume->SetDataDynamicGetter(MAKE_OPTION_DATA_CONTROL(GetOverallVolume));
			OverallVolume->SetDataDynamicSetter(MAKE_OPTION_DATA_CONTROL(SetOverallVolume));
			OverallVolume->SetShouldApplyChangeImimediately(true);
			//TODO :: Set data dynamic getter and setter for the data object

			VolumeCategoryCollection->AddChildListData(OverallVolume);
			
		}
		
	}
	RegisteredOptionsTabCollections.Add(AudioTabCollection);
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
