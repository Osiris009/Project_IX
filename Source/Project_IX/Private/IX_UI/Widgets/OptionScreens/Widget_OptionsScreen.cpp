// Fill out your copyright notice in the Description page of Project Settings.


#include "IX_UI/Widgets/OptionScreens/Widget_OptionsScreen.h"
#include "Input/CommonUIInputTypes.h"
#include "ICommonInputModule.h" 
#include "ProjectDebugHelper.h"

#include "IX_UI/Widgets/OptionScreens/OptionsDataRegistry.h"
#include "IX_UI/Widgets/Components/ProIXTabListWidgetBase.h"
#include "IX_UI/Widgets/OptionScreens/DataObjects/ListDataObject_Collection.h"
#include "IX_UI/Widgets/Components/PIX_CommonListView.h"



void UWidget_OptionsScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (!ResetAction.IsNull())
	{
		 ResetActionHandle = RegisterUIActionBinding(
			FBindUIActionArgs(   

				ResetAction,
				true, // bShouldDisplayInActionBar
				FSimpleDelegate::CreateUObject(this, &ThisClass::OnResetBoundActionTriggered)
				// Note: We intentionally don't bind any hold progress or hold start /
				// end delegates here, as resetting options is intended to be an instant action with no hold requirement.
				// If we find that we want to add a hold requirement later, we can always add those delegates in then.
			)
		);


	}

	RegisterUIActionBinding(
		FBindUIActionArgs(
			ICommonInputModule::GetSettings().GetDefaultBackAction(),
			true,
			FSimpleDelegate::CreateUObject(this, &ThisClass::OnBackBoundActionTriggered)
			)
	);

	// Bind the tab selection delegate once during initialization, BEFORE any tabs are registered.
	// This ensures we catch the auto-selection that CommonUI fires during RegisterTab.
	TabListWidget_OptionsTabs->OnTabSelected.AddDynamic(this, &ThisClass::OnOptionsTabSelected);
}

void UWidget_OptionsScreen::NativeOnActivated()
{
	Super::NativeOnActivated();

	FName FirstTabID = NAME_None;

	for (UListDataObject_Collection* TabCollection : GetorCreateDataRegistry()->GetRegisteredOptionsTabCollections())
	{
		if (!TabCollection) continue;

		const FName TabID = TabCollection->GetDataID();

		if (TabListWidget_OptionsTabs->GetTabButtonBaseByID(TabID) != nullptr)
		{
			continue;
		}

		TabListWidget_OptionsTabs->RequestRegisterTab(TabID, TabCollection->GetDataDisplayName());

		// Track the first registered tab
		if (FirstTabID == NAME_None)
		{
			FirstTabID = TabID;
		}
	}

	// Explicitly populate the list for the first tab.
	// We call OnOptionsTabSelected directly because SetActiveTab may be a no-op 
	// if CommonUI already considers this tab selected from RegisterTab's auto-selection.
	if (FirstTabID != NAME_None)
	{
		//
		OnOptionsTabSelected(FirstTabID);
	}
}

UOptionsDataRegistry* UWidget_OptionsScreen::GetorCreateDataRegistry()
{
	// If we already have a valid data registry, return it

	if (!CreatedOwningDataRegistry)
	{
		CreatedOwningDataRegistry = NewObject<UOptionsDataRegistry>(this);
		CreatedOwningDataRegistry->InitOptionsDataRegistry(GetOwningLocalPlayer());

	}

	checkf(CreatedOwningDataRegistry, TEXT("Options screen data registry was not created successfully!"));

	
	return CreatedOwningDataRegistry;
}

void UWidget_OptionsScreen::OnOptionsTabSelected(FName TabID)
{
	Debug::Print(FString::Printf(TEXT("Options tab selected with ID: %s"), *TabID.ToString()));
	TArray<UListDataObject_Base*> FoundListSourceItems = GetorCreateDataRegistry()->GetListSourceItemsBySelectedTabID(TabID);

	CommonListView_OptionsList->SetListItems(FoundListSourceItems);

	CommonListView_OptionsList->RequestRefresh();

	if (CommonListView_OptionsList->GetNumItems() != 0)
	{
		CommonListView_OptionsList->NavigateToIndex(0);
		CommonListView_OptionsList->SetSelectedIndex(0);
	}
}

void UWidget_OptionsScreen::OnResetBoundActionTriggered()
{
	Debug::Print(TEXT("Reset action triggered on options screen! Implement reset logic here."));
}

void UWidget_OptionsScreen::OnBackBoundActionTriggered()
{
	DeactivateWidget();

}
 