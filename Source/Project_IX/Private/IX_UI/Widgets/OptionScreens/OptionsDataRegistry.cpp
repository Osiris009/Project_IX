// Fill out your copyright notice in the Description page of Project Settings.


#include "IX_UI/Widgets/OptionScreens/OptionsDataRegistry.h"

#include "IX_UI/Widgets/OptionScreens/DataObjects/ListDataObject_Collection.h"
#include "IX_UI/Widgets/OptionScreens/DataObjects/ListDataObject_String.h"

#include "IX_UI/Widgets/OptionScreens/OptionDataInterationHelper.h"
#include "IXProjectSettings/PIXGameUserSettings.h"
#include "IX_UI/Extra/UIGamePlayTags.h"
#include "IXFuctionLibrary/UIFunctionLibrary.h"
#include "IX_UI/Widgets/OptionScreens/DataObjects/ListDataObject_Scalar.h"

#include "IX_UI/Widgets/OptionScreens/DataObjects/ListDataObject_StringResolution.h"
//For Reading the string table for the description of the options
#include "Internationalization/StringTableRegistry.h"



#define MAKE_OPTION_DATA_CONTROL(SetterOrGetterFuncName) \
	MakeShared<FOptionDataInterationHelper>(GET_FUNCTION_NAME_STRING_CHECKED(UPIXGameUserSettings, SetterOrGetterFuncName))

#define GET_DESCRIPTION(InKey) LOCTABLE("/Game/ProjectIX/UI/StringTables/ST_OptionsScreenDescription.ST_OptionsScreenDescription", InKey)


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
		GameDifficulty->SetDescriptionRichText(FText::FromString(TEXT("Adjusts the difficulty of the game experience.\n\n<Bold>Easy:</> Focuses on the story experience. Provides the most relaxing combat.\n\n<Bold>Medium:</> Offers slightly harder combat experience\n\n<Bold>Hard:</>Offers a much more challenging combat experience\n\n<Bold>Vert Hard:</> Provides the most challenging combat experience. Not recommended for first play through.")));
		GameDifficulty->AddDynamicOption(TEXT("Easy"), FText::FromString(TEXT("Easy")));
		GameDifficulty->AddDynamicOption(TEXT("Medium"), FText::FromString(TEXT("Medium")));
		GameDifficulty->AddDynamicOption(TEXT("Hard"), FText::FromString(TEXT("Hard")));
		GameDifficulty->AddDynamicOption(TEXT("VeryHard"), FText::FromString(TEXT("VeryHard")));

		GameDifficulty->SetDefaultValueFromString(TEXT("Medium")); // Set the default value for the GameDifficulty option


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
		
		{
			UListDataObject_Scalar* MusicVolume = NewObject<UListDataObject_Scalar>();
			MusicVolume->SetDataID(FName("MusicVolume"));
			MusicVolume->SetDataDisplayName(FText::FromString(TEXT("Music Volume")));
			MusicVolume->SetDescriptionRichText(FText::FromString(TEXT("This is description for Music Volume")));
			MusicVolume->SetDisplayValueRange(TRange<float>(0.f,1.f));
			MusicVolume->SetOutputValueRange(TRange<float>(0.f,2.f));
			MusicVolume->SetSliderStepSize(0.01f);
			MusicVolume->SetDefaultValueFromString(LexToString(1.f));
			MusicVolume->SetDisplayNumericType(ECommonNumericType::Percentage);
			MusicVolume->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());  //No Decimal: 50%  //One Decimal: 50.5%
			MusicVolume->SetDataDynamicGetter(MAKE_OPTION_DATA_CONTROL(GetMusicVolume));
			MusicVolume->SetDataDynamicSetter(MAKE_OPTION_DATA_CONTROL(SetMusicVolume));
			MusicVolume->SetShouldApplyChangeImimediately(true);

			VolumeCategoryCollection->AddChildListData(MusicVolume);
		}
		
		{
			UListDataObject_Scalar* SFXVolume = NewObject<UListDataObject_Scalar>();
			SFXVolume->SetDataID(FName("SFXVolume"));
			SFXVolume->SetDataDisplayName(FText::FromString(TEXT("SFX Volume")));
			SFXVolume->SetDescriptionRichText(FText::FromString(TEXT("This is description for SFX Volume")));
			SFXVolume->SetDisplayValueRange(TRange<float>(0.f,1.f));
			SFXVolume->SetOutputValueRange(TRange<float>(0.f,2.f));
			SFXVolume->SetSliderStepSize(0.01f);
			SFXVolume->SetDefaultValueFromString(LexToString(1.f));
			SFXVolume->SetDisplayNumericType(ECommonNumericType::Percentage);
			SFXVolume->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());  //No Decimal: 50%  //One Decimal: 50.5%
			
			SFXVolume->SetDataDynamicGetter(MAKE_OPTION_DATA_CONTROL(GetSFXVolume));
			SFXVolume->SetDataDynamicSetter(MAKE_OPTION_DATA_CONTROL(SetSFXVolume));
			SFXVolume->SetShouldApplyChangeImimediately(true);
			//TODO :: Set data dynamic getter and setter for the data object

			VolumeCategoryCollection->AddChildListData(SFXVolume);
		}
		
	}
	
	//Sound Category
	{
		UListDataObject_Collection* SoundCategoryCollection = NewObject<UListDataObject_Collection>();
		SoundCategoryCollection->SetDataID(FName("SoundCategoryCollection"));
		SoundCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("Sound")));
		AudioTabCollection->AddChildListData(SoundCategoryCollection);
		
		//Allow Back Graound Sound
		{
			UListDataObject_StringBool* AllowBackgroundAudio = NewObject<UListDataObject_StringBool>();
			AllowBackgroundAudio->SetDataID(FName("AllowBackgroundAudio"));
			AllowBackgroundAudio->SetDataDisplayName(FText::FromString(TEXT("Allow Background Audio")));
			AllowBackgroundAudio->OverrideTrueDisplayText(FText::FromString(TEXT("Enabled")));
			AllowBackgroundAudio->OverrideFalseDisplayText(FText::FromString(TEXT("Disabled")));
			AllowBackgroundAudio->SetFalseAsDefaultValue();
			AllowBackgroundAudio->SetDataDynamicGetter(MAKE_OPTION_DATA_CONTROL(GetAllowBackgroundAudio));
			AllowBackgroundAudio->SetDataDynamicSetter(MAKE_OPTION_DATA_CONTROL(SetAllowBackgroundAudio));
			AllowBackgroundAudio->SetShouldApplyChangeImimediately(true);
			
			SoundCategoryCollection->AddChildListData(AllowBackgroundAudio);
		}
		
		{
			UListDataObject_StringBool* UseHDRAudioMode = NewObject<UListDataObject_StringBool>();
			UseHDRAudioMode->SetDataID(FName("UseHDRAudioMode"));
			UseHDRAudioMode->SetDataDisplayName(FText::FromString(TEXT("Use HDR Audio Mode")));
			UseHDRAudioMode->OverrideTrueDisplayText(FText::FromString(TEXT("Enabled")));
			UseHDRAudioMode->OverrideFalseDisplayText(FText::FromString(TEXT("Disabled")));
			UseHDRAudioMode->SetFalseAsDefaultValue();
			UseHDRAudioMode->SetDataDynamicGetter(MAKE_OPTION_DATA_CONTROL(GetUseHDRAudioMode));
			UseHDRAudioMode->SetDataDynamicSetter(MAKE_OPTION_DATA_CONTROL(SetUseHDRAudioMode));
			UseHDRAudioMode->SetShouldApplyChangeImimediately(true);
			
			SoundCategoryCollection->AddChildListData(UseHDRAudioMode);
		}
	}
	RegisteredOptionsTabCollections.Add(AudioTabCollection);
}

void UOptionsDataRegistry::InitVideoCollectionTab()
{
	UListDataObject_Collection* VideoCollectionTab = NewObject<UListDataObject_Collection>();
	VideoCollectionTab->SetDataID(FName("VideoCollectionTab"));
	VideoCollectionTab->SetDataDisplayName(FText::FromString("Video"));

	UListDataObject_StringEnum* CreatedWindowMode = nullptr;

	{
		UListDataObject_Collection* DisplayCategoryCollection = NewObject<UListDataObject_Collection>();
		DisplayCategoryCollection->SetDataID(FName("DisplayCategoryCollection"));
		DisplayCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("Display")));

		VideoCollectionTab->AddChildListData(DisplayCategoryCollection);

		FOptionsDataEditConditionDescriptor PackegedBuildOnlyCondition;
		PackegedBuildOnlyCondition.SetEditConditionFunc(
			[]()->bool
			{
				const bool bIsInEditor = GIsEditor || GIsPlayInEditorWorld;

				return !bIsInEditor;
			}
		);

		PackegedBuildOnlyCondition.SetDisabledRichReason(TEXT("\n\n<Disabled>This setting can only be adjusted in a packaged build.</>"));

		//Window Mode
		{
			UListDataObject_StringEnum* WindowMode = NewObject<UListDataObject_StringEnum>();
			WindowMode->SetDataID(FName("WindowMode"));
			WindowMode->SetDataDisplayName(FText::FromString(TEXT("Window Mode")));
			WindowMode->SetDescriptionRichText(GET_DESCRIPTION("WindowModeDescKey"));
			WindowMode->AddEnumOption(EWindowMode::Fullscreen, FText::FromString(TEXT("Fullscreen Mode")));
			WindowMode->AddEnumOption(EWindowMode::WindowedFullscreen, FText::FromString(TEXT("Borderless Mode")));
			WindowMode->AddEnumOption(EWindowMode::Windowed, FText::FromString(TEXT("Windowed Mode")));
			WindowMode->SetDefaultValueFromEnumOption(EWindowMode::WindowedFullscreen);
			WindowMode->SetDataDynamicGetter(MAKE_OPTION_DATA_CONTROL(GetFullscreenMode));
			WindowMode->SetDataDynamicSetter(MAKE_OPTION_DATA_CONTROL(SetFullscreenMode));
			WindowMode->SetShouldApplyChangeImimediately(true);

			WindowMode->AddEditCondition(PackegedBuildOnlyCondition);

			CreatedWindowMode = WindowMode;

			DisplayCategoryCollection->AddChildListData(WindowMode);

		}

		//Screen Resultion
		{
			UListDataObject_StringResolution* ScreenResolution = NewObject<UListDataObject_StringResolution>();
			ScreenResolution->SetDataID(FName("ScreenResolution"));
			ScreenResolution->SetDataDisplayName(FText::FromString(TEXT("Screen Resolution")));
			ScreenResolution->SetDescriptionRichText(GET_DESCRIPTION("ScreenResolutionsDescKey"));
			ScreenResolution->InitResolutionValues();
			ScreenResolution->SetDataDynamicGetter(MAKE_OPTION_DATA_CONTROL(GetScreenResolution));
			ScreenResolution->SetDataDynamicSetter(MAKE_OPTION_DATA_CONTROL(SetScreenResolution));
			ScreenResolution->SetShouldApplyChangeImimediately(true);

			ScreenResolution->AddEditCondition(PackegedBuildOnlyCondition);

			FOptionsDataEditConditionDescriptor WindowModeEditCondition;
			WindowModeEditCondition.SetEditConditionFunc(
				[CreatedWindowMode]()->bool
				{
					const bool bIsBoarderlessWindow = CreatedWindowMode->GetCurrentValueAsEnum<EWindowMode::Type>() == EWindowMode::WindowedFullscreen;

					return !bIsBoarderlessWindow;
				}

			);

			// Set the disabled reason and forced value for the ScreenResolution option when the edit condition is not met
			WindowModeEditCondition.SetDisabledRichReason(TEXT("\n\n<Disabled>Screen Resolution is not adjustable when the 'Window Mode' is set to Borderless Window.The value must match with the maximum allowed resolution.</>"));
			WindowModeEditCondition.SetDisabledForcedStringValue(ScreenResolution->GetMaximumAllowedResolution());
			// Add the WindowModeEditCondition to the ScreenResolution option
			ScreenResolution->AddEditCondition(WindowModeEditCondition);
			// Add the WindowMode as a dependency for the ScreenResolution option
			ScreenResolution->AddEditDependencyData(CreatedWindowMode);
			// Add the ScreenResolution option to the DisplayCategoryCollection
			DisplayCategoryCollection->AddChildListData(ScreenResolution);


		}

		////Graphics Category
		{
			UListDataObject_Collection* GraphicsCategoryCollection = NewObject<UListDataObject_Collection>();
			GraphicsCategoryCollection->SetDataID(FName("GraphicsCategoryCollection"));
			GraphicsCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("Graphics")));

			VideoCollectionTab->AddChildListData(GraphicsCategoryCollection);


			//Display Gamma
			{
				UListDataObject_Scalar* DisplayGamma = NewObject<UListDataObject_Scalar>();
				DisplayGamma->SetDataID(FName("DisplayGamma"));
				DisplayGamma->SetDataDisplayName(FText::FromString(TEXT("Brightness")));
				DisplayGamma->SetDescriptionRichText(GET_DESCRIPTION("DisplayGammaDescKey"));
				DisplayGamma->SetDisplayValueRange(TRange<float>(0.f, 1.f));
				//The default value Unreal has is: 2.2f
				DisplayGamma->SetOutputValueRange(TRange<float>(1.7f, 2.7f));
				DisplayGamma->SetSliderStepSize(0.01f);
				DisplayGamma->SetDisplayNumericType(ECommonNumericType::Percentage);
				DisplayGamma->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());
				DisplayGamma->SetDataDynamicGetter(MAKE_OPTION_DATA_CONTROL(GetCurrentDisplayGamma));
				DisplayGamma->SetDataDynamicSetter(MAKE_OPTION_DATA_CONTROL(SetCurrentDisplayGamma));
				DisplayGamma->SetDefaultValueFromString(LexToString(2.2f));

				GraphicsCategoryCollection->AddChildListData(DisplayGamma);

			}

			UListDataObject_StringInteger* CreatedOverallQuality = nullptr;

			//OverAll Graphics Quality
			{
				UListDataObject_StringInteger* OverallQuality = NewObject<UListDataObject_StringInteger>();
				OverallQuality->SetDataID(FName("OverallQuality"));
				OverallQuality->SetDataDisplayName(FText::FromString(TEXT("Overall Quality")));
				OverallQuality->SetDescriptionRichText(GET_DESCRIPTION("OverallQualityDescKey"));

				OverallQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
				OverallQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
				OverallQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
				OverallQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
				OverallQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));

				OverallQuality->SetDataDynamicGetter(MAKE_OPTION_DATA_CONTROL(GetOverallScalabilityLevel));
				OverallQuality->SetDataDynamicSetter(MAKE_OPTION_DATA_CONTROL(SetOverallScalabilityLevel));

				OverallQuality->SetShouldApplyChangeImimediately(true);
				//TODO :: Set data dynamic getter and setter for the data object
				GraphicsCategoryCollection->AddChildListData(OverallQuality);

				CreatedOverallQuality = OverallQuality;
			}

			//Resolution Scale
			{
				UListDataObject_Scalar* ResolutionScale = NewObject<UListDataObject_Scalar>();
				ResolutionScale->SetDataID(FName("ResolutionScale"));
				ResolutionScale->SetDataDisplayName(FText::FromString(TEXT("3D Resolution")));
				ResolutionScale->SetDescriptionRichText(GET_DESCRIPTION("ResolutionScaleDescKey"));
				ResolutionScale->SetDisplayValueRange(TRange<float>(0.f, 1.f));
				ResolutionScale->SetOutputValueRange(TRange<float>(0.f, 1.f));
				ResolutionScale->SetSliderStepSize(0.01f);
				ResolutionScale->SetDisplayNumericType(ECommonNumericType::Percentage);
				ResolutionScale->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());
				ResolutionScale->SetDataDynamicGetter(MAKE_OPTION_DATA_CONTROL(GetResolutionScaleNormalized));
				ResolutionScale->SetDataDynamicSetter(MAKE_OPTION_DATA_CONTROL(SetResolutionScaleNormalized));
				ResolutionScale->SetShouldApplyChangeImimediately(true);

				ResolutionScale->AddEditDependencyData(CreatedOverallQuality);

				GraphicsCategoryCollection->AddChildListData(ResolutionScale);
			}

			//Global Illumination
			{
				UListDataObject_StringInteger* GlobalIlluminationQuality = NewObject<UListDataObject_StringInteger>();
				GlobalIlluminationQuality->SetDataID(FName("GlobalIlluminationQuality"));
				GlobalIlluminationQuality->SetDataDisplayName(FText::FromString(TEXT("Global Illumination")));
				GlobalIlluminationQuality->SetDescriptionRichText(GET_DESCRIPTION("GlobalIlluminationQualityDescKey"));

				GlobalIlluminationQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
				GlobalIlluminationQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
				GlobalIlluminationQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
				GlobalIlluminationQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
				GlobalIlluminationQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));

				GlobalIlluminationQuality->SetDataDynamicGetter(MAKE_OPTION_DATA_CONTROL(GetGlobalIlluminationQuality));
				GlobalIlluminationQuality->SetDataDynamicSetter(MAKE_OPTION_DATA_CONTROL(SetGlobalIlluminationQuality));

				GlobalIlluminationQuality->SetShouldApplyChangeImimediately(true);

				GlobalIlluminationQuality->AddEditDependencyData(CreatedOverallQuality);

				CreatedOverallQuality->AddEditDependencyData(GlobalIlluminationQuality);

				GraphicsCategoryCollection->AddChildListData(GlobalIlluminationQuality);
			}

			//Shadow Quality
			{
				UListDataObject_StringInteger* ShadowQuality = NewObject<UListDataObject_StringInteger>();
				ShadowQuality->SetDataID(FName("ShadowQuality"));
				ShadowQuality->SetDataDisplayName(FText::FromString(TEXT("Shadow Quality")));
				ShadowQuality->SetDescriptionRichText(GET_DESCRIPTION("ShadowQualityDescKey"));

				ShadowQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
				ShadowQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
				ShadowQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
				ShadowQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
				ShadowQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));

				ShadowQuality->SetDataDynamicGetter(MAKE_OPTION_DATA_CONTROL(GetShadowQuality));
				ShadowQuality->SetDataDynamicSetter(MAKE_OPTION_DATA_CONTROL(SetShadowQuality));

				ShadowQuality->SetShouldApplyChangeImimediately(true);

				ShadowQuality->AddEditDependencyData(CreatedOverallQuality);

				CreatedOverallQuality->AddEditDependencyData(ShadowQuality);

				GraphicsCategoryCollection->AddChildListData(ShadowQuality);
			}


			//AntiAliasing Quality
			{
				UListDataObject_StringInteger* AntiAliasingQuality = NewObject<UListDataObject_StringInteger>();
				AntiAliasingQuality->SetDataID(FName("AntiAliasingQuality"));
				AntiAliasingQuality->SetDataDisplayName(FText::FromString(TEXT("Anti-Aliasing Quality")));
				AntiAliasingQuality->SetDescriptionRichText(GET_DESCRIPTION("AntiAliasingDescKey"));

				AntiAliasingQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
				AntiAliasingQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
				AntiAliasingQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
				AntiAliasingQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
				AntiAliasingQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));

				AntiAliasingQuality->SetDataDynamicGetter(MAKE_OPTION_DATA_CONTROL(GetAntiAliasingQuality));
				AntiAliasingQuality->SetDataDynamicSetter(MAKE_OPTION_DATA_CONTROL(SetAntiAliasingQuality));

				AntiAliasingQuality->SetShouldApplyChangeImimediately(true);

				AntiAliasingQuality->AddEditDependencyData(CreatedOverallQuality);

				CreatedOverallQuality->AddEditDependencyData(AntiAliasingQuality);

				GraphicsCategoryCollection->AddChildListData(AntiAliasingQuality);
			}

			//View Distance Quality
			{
				UListDataObject_StringInteger* ViewDistanceQuality = NewObject<UListDataObject_StringInteger>();
				ViewDistanceQuality->SetDataID(FName("ViewDistanceQuality"));
				ViewDistanceQuality->SetDataDisplayName(FText::FromString(TEXT("View Distance Quality")));
				ViewDistanceQuality->SetDescriptionRichText(GET_DESCRIPTION("ViewDistanceDescKey"));

				ViewDistanceQuality->AddIntegerOption(0, FText::FromString(TEXT("Near")));
				ViewDistanceQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
				ViewDistanceQuality->AddIntegerOption(2, FText::FromString(TEXT("Far")));
				ViewDistanceQuality->AddIntegerOption(3, FText::FromString(TEXT("Very Far")));
				ViewDistanceQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));

				ViewDistanceQuality->SetDataDynamicGetter(MAKE_OPTION_DATA_CONTROL(GetViewDistanceQuality));
				ViewDistanceQuality->SetDataDynamicSetter(MAKE_OPTION_DATA_CONTROL(SetViewDistanceQuality));

				ViewDistanceQuality->SetShouldApplyChangeImimediately(true);

				ViewDistanceQuality->AddEditDependencyData(CreatedOverallQuality);

				CreatedOverallQuality->AddEditDependencyData(ViewDistanceQuality);

				GraphicsCategoryCollection->AddChildListData(ViewDistanceQuality);
			}

			{
				UListDataObject_StringInteger* TextureQuality = NewObject<UListDataObject_StringInteger>();
				TextureQuality->SetDataID(FName("TextureQuality"));
				TextureQuality->SetDataDisplayName(FText::FromString(TEXT("Texture Quality")));
				TextureQuality->SetDescriptionRichText(GET_DESCRIPTION("TextureQualityDescKey"));
				TextureQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
				TextureQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
				TextureQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
				TextureQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
				TextureQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));
				TextureQuality->SetDataDynamicGetter(MAKE_OPTION_DATA_CONTROL(GetTextureQuality));
				TextureQuality->SetDataDynamicSetter(MAKE_OPTION_DATA_CONTROL(SetTextureQuality));
				TextureQuality->SetShouldApplyChangeImimediately(true);

				TextureQuality->AddEditDependencyData(CreatedOverallQuality);

				CreatedOverallQuality->AddEditDependencyData(TextureQuality);

				GraphicsCategoryCollection->AddChildListData(TextureQuality);
			}

			//Visual Effects Quality
			{
				UListDataObject_StringInteger* VisualEffectQuality = NewObject<UListDataObject_StringInteger>();
				VisualEffectQuality->SetDataID(FName("VisualEffectQuality"));
				VisualEffectQuality->SetDataDisplayName(FText::FromString(TEXT("Visual Effect Quality")));
				VisualEffectQuality->SetDescriptionRichText(GET_DESCRIPTION("VisualEffectQualityDescKey"));
				VisualEffectQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
				VisualEffectQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
				VisualEffectQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
				VisualEffectQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
				VisualEffectQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));
				VisualEffectQuality->SetDataDynamicGetter(MAKE_OPTION_DATA_CONTROL(GetVisualEffectQuality));
				VisualEffectQuality->SetDataDynamicSetter(MAKE_OPTION_DATA_CONTROL(SetVisualEffectQuality));
				VisualEffectQuality->SetShouldApplyChangeImimediately(true);

				VisualEffectQuality->AddEditDependencyData(CreatedOverallQuality);

				CreatedOverallQuality->AddEditDependencyData(VisualEffectQuality);

				GraphicsCategoryCollection->AddChildListData(VisualEffectQuality);
			}

			//Reflection Quality
			{
				UListDataObject_StringInteger* ReflectionQuality = NewObject<UListDataObject_StringInteger>();
				ReflectionQuality->SetDataID(FName("ReflectionQuality"));
				ReflectionQuality->SetDataDisplayName(FText::FromString(TEXT("Reflection Quality")));
				ReflectionQuality->SetDescriptionRichText(GET_DESCRIPTION("ReflectionQualityDescKey"));
				ReflectionQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
				ReflectionQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
				ReflectionQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
				ReflectionQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
				ReflectionQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));
				ReflectionQuality->SetDataDynamicGetter(MAKE_OPTION_DATA_CONTROL(GetReflectionQuality));
				ReflectionQuality->SetDataDynamicSetter(MAKE_OPTION_DATA_CONTROL(SetReflectionQuality));
				ReflectionQuality->SetShouldApplyChangeImimediately(true);

				ReflectionQuality->AddEditDependencyData(CreatedOverallQuality);

				CreatedOverallQuality->AddEditDependencyData(ReflectionQuality);

				GraphicsCategoryCollection->AddChildListData(ReflectionQuality);
			}

			//Post Processing Quality
			{
				UListDataObject_StringInteger* PostProcessingQuality = NewObject<UListDataObject_StringInteger>();
				PostProcessingQuality->SetDataID(FName("PostProcessingQuality"));
				PostProcessingQuality->SetDataDisplayName(FText::FromString(TEXT("Post Processing Quality")));
				PostProcessingQuality->SetDescriptionRichText(GET_DESCRIPTION("PostProcessingQualityDescKey"));
				PostProcessingQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
				PostProcessingQuality->AddIntegerOption(1, FText::FromString(TEXT("Medium")));
				PostProcessingQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
				PostProcessingQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
				PostProcessingQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));
				PostProcessingQuality->SetDataDynamicGetter(MAKE_OPTION_DATA_CONTROL(GetPostProcessingQuality));
				PostProcessingQuality->SetDataDynamicSetter(MAKE_OPTION_DATA_CONTROL(SetPostProcessingQuality));
				PostProcessingQuality->SetShouldApplyChangeImimediately(true);

				PostProcessingQuality->AddEditDependencyData(CreatedOverallQuality);

				CreatedOverallQuality->AddEditDependencyData(PostProcessingQuality);

				GraphicsCategoryCollection->AddChildListData(PostProcessingQuality);
			}

		}

		//Advanced Graphics Category
		{
			UListDataObject_Collection* AdvancedGraphicsCategoryCollection = NewObject<UListDataObject_Collection>();
			AdvancedGraphicsCategoryCollection->SetDataID(FName("AdvancedGraphicsCategoryCollection"));
			AdvancedGraphicsCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("Advanced Graphics")));

			VideoCollectionTab->AddChildListData(AdvancedGraphicsCategoryCollection);

			//Vertical Sync 
			{
				UListDataObject_StringBool* VerticalSync = NewObject<UListDataObject_StringBool>();
				VerticalSync->SetDataID(FName("VerticalSync"));
				VerticalSync->SetDataDisplayName(FText::FromString(TEXT("V-Sync")));
				VerticalSync->SetDescriptionRichText(GET_DESCRIPTION("VerticalSyncDescKey"));
				VerticalSync->SetDataDynamicGetter(MAKE_OPTION_DATA_CONTROL(IsVSyncEnabled));
				VerticalSync->SetDataDynamicSetter(MAKE_OPTION_DATA_CONTROL(SetVSyncEnabled));
				VerticalSync->SetFalseAsDefaultValue();
				VerticalSync->SetShouldApplyChangeImimediately(true);

				FOptionsDataEditConditionDescriptor FullScreenOnlyCondition;
				FullScreenOnlyCondition.SetEditConditionFunc(
					[CreatedWindowMode]()->bool
					{
						return CreatedWindowMode->GetCurrentValueAsEnum<EWindowMode::Type>() == EWindowMode::Fullscreen;

					}
				);

				FullScreenOnlyCondition.SetDisabledRichReason(TEXT("\n\n<Disabled>This feature only works if the 'Window Mode' is set to 'Fullscreen'.</>"));
				FullScreenOnlyCondition.SetDisabledForcedStringValue(TEXT("false"));

				VerticalSync->AddEditCondition(FullScreenOnlyCondition);

				AdvancedGraphicsCategoryCollection->AddChildListData(VerticalSync);
			}

			//Frame Rate Limit
			{
				UListDataObject_String* FrameRateLimit = NewObject<UListDataObject_String>();
				FrameRateLimit->SetDataID(FName("FrameRateLimit"));
				FrameRateLimit->SetDataDisplayName(FText::FromString(TEXT("Frame Rate Limit")));
				FrameRateLimit->SetDescriptionRichText(GET_DESCRIPTION("FrameRateLimitDescKey"));
				FrameRateLimit->AddDynamicOption(LexToString(30.f), FText::FromString(TEXT("30 FPS")));
				FrameRateLimit->AddDynamicOption(LexToString(60.f), FText::FromString(TEXT("60 FPS")));
				FrameRateLimit->AddDynamicOption(LexToString(90.f), FText::FromString(TEXT("90 FPS")));
				FrameRateLimit->AddDynamicOption(LexToString(120.f), FText::FromString(TEXT("120 FPS")));
				FrameRateLimit->AddDynamicOption(LexToString(0.f), FText::FromString(TEXT("No Limit")));
				FrameRateLimit->SetDefaultValueFromString(LexToString(0.f));
				FrameRateLimit->SetDataDynamicGetter(MAKE_OPTION_DATA_CONTROL(GetFrameRateLimit));
				FrameRateLimit->SetDataDynamicSetter(MAKE_OPTION_DATA_CONTROL(SetFrameRateLimit));
				FrameRateLimit->SetShouldApplyChangeImimediately(true);

				AdvancedGraphicsCategoryCollection->AddChildListData(FrameRateLimit);
			}


		}
		RegisteredOptionsTabCollections.Add(VideoCollectionTab);


	}

}


void UOptionsDataRegistry::InitControlsCollectionTab()
{
	UListDataObject_Collection* ControlsCollectionTab = NewObject<UListDataObject_Collection>();
	ControlsCollectionTab->SetDataID(FName("ControlsCollectionTab"));
	ControlsCollectionTab->SetDataDisplayName(FText::FromString("Controls"));	
	RegisteredOptionsTabCollections.Add(ControlsCollectionTab);
}
