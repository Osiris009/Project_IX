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

}

void UWidget_OptionsScreen::NativeOnActivated()
{
	Super::NativeOnActivated();

	/*At this point, the options screen should have its data registry ready to go,
	 so we can broadcast an event or call a function to notify any child widgets
	 (like the options tabs) that they can now access the data registry and 
	 get the data they need to populate themselves*/


	for (UListDataObject_Collection* TabCollection : GetorCreateDataRegistry()->GetRegisteredOptionsTabCollections())
	{
		if (!TabCollection)
		{
			continue;
		}
		const FName TabID = TabCollection->GetDataID();

		if ( TabListWidget_OptionsTabs->GetTabButtonBaseByID(TabID) != nullptr)
		{
			// Do something with the TabButton if needed
			continue;
		}

		TabListWidget_OptionsTabs->RequestRegisterTab(TabID, TabCollection->GetDataDisplayName());
	}

	TabListWidget_OptionsTabs->OnTabSelected.AddDynamic(this, &ThisClass::OnOptionsTabSelected);

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
 