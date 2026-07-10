// Fill out your copyright notice in the Description page of Project Settings.


#include "IX_UI/Widgets/OptionScreens/Widget_OptionsScreen.h"
#include "Input/CommonUIInputTypes.h"
#include "ICommonInputModule.h" 
#include "ProjectDebugHelper.h"

#include "IX_UI/Widgets/OptionScreens/OptionsDataRegistry.h"
#include "IX_UI/Widgets/Components/ProIXTabListWidgetBase.h"
#include "IX_UI/Widgets/OptionScreens/DataObjects/ListDataObject_Collection.h"
#include "IX_UI/Widgets/Components/PIX_CommonListView.h"
#include "IXProjectSettings/PIXGameUserSettings.h"

#include "IX_UI/Widgets/OptionScreens/ListEntries/Widget_ListEntry_Base.h"
#include "IX_UI/Widgets/OptionScreens/Widget_OptionsDetailsView.h"
#include "IX_UI/IX_UISubsystem.h"
#include "IX_UI/Widgets/Components/PIX_CommonButtonBase.h"


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

	CommonListView_OptionsList->OnItemIsHoveredChanged().AddUObject(this, &ThisClass::OnListViewItemHovered);

	CommonListView_OptionsList->OnItemSelectionChanged().AddUObject(this, &ThisClass::OnListViewItemSelected);

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

UWidget* UWidget_OptionsScreen::NativeGetDesiredFocusTarget() const
{
	if (UObject* SelectedItem = CommonListView_OptionsList->GetSelectedItem())
	{
		if (UUserWidget* SelectedEntryWidget = CommonListView_OptionsList->GetEntryWidgetFromItem(SelectedItem))
		{
			return SelectedEntryWidget;
		}
	}
	return Super::NativeGetDesiredFocusTarget();
}

void UWidget_OptionsScreen::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();
	// Apply the settings when the options screen is deactivated
	UPIXGameUserSettings::Get()->ApplySettings(true);
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
	DetailesView_ListEntryInfo->ClearDetailsViewInfo();

	TArray<UListDataObject_Base*> FoundListSourceItems = GetorCreateDataRegistry()->GetListSourceItemsBySelectedTabID(TabID);

	CommonListView_OptionsList->SetListItems(FoundListSourceItems);

	CommonListView_OptionsList->RequestRefresh();

	if (CommonListView_OptionsList->GetNumItems() != 0)
	{
		CommonListView_OptionsList->NavigateToIndex(0);
		CommonListView_OptionsList->SetSelectedIndex(0);
	}

	ResettableDataArray.Empty();

	for (UListDataObject_Base* FoundListSourceItem : FoundListSourceItems)
	{
		if (!FoundListSourceItem)
		{
			continue;
		}

		if (!FoundListSourceItem->OnListDataModified.IsBoundToObject(this))
		{
			FoundListSourceItem->OnListDataModified.AddUObject(this, &ThisClass::OnListViewListDataModified);
		}

		if (FoundListSourceItem->CanResetBackToDefaultValue())
		{
			ResettableDataArray.AddUnique(FoundListSourceItem);
		}
	}

	if (ResettableDataArray.IsEmpty())
	{
		RemoveActionBinding(ResetActionHandle);
	}
	else
	{
		if (!GetActionBindings().Contains(ResetActionHandle))
		{
			AddActionBinding(ResetActionHandle);
		}
	}
}

void UWidget_OptionsScreen::OnListViewItemHovered(UObject* InHoveredItem, bool bWasHovered)
{
	if (!InHoveredItem)
	{
		return;
	}
	
	UWidget_ListEntry_Base* HoveredEntryWidget = CommonListView_OptionsList->GetEntryWidgetFromItem<UWidget_ListEntry_Base>(InHoveredItem);
	if (HoveredEntryWidget)
	{
		HoveredEntryWidget->NativeOnListEntryWidgetHovered(bWasHovered);
	}

	if (bWasHovered)
	{
		DetailesView_ListEntryInfo->UpdateDetailsViewInfo(
			CastChecked<UListDataObject_Base>(InHoveredItem),
			TryGetEntryWidgetClassName(InHoveredItem)
		);
	}

	else
	{
		if(UListDataObject_Base * SelectedItem = CommonListView_OptionsList->GetSelectedItem<UListDataObject_Base>())
		{
			DetailesView_ListEntryInfo->UpdateDetailsViewInfo(
				SelectedItem,
				TryGetEntryWidgetClassName(SelectedItem)
			);
		}
	}
}

void UWidget_OptionsScreen::OnListViewItemSelected(UObject* InSelectedItem)
{
	if (!InSelectedItem)
	{
		return;
	}
	
	DetailesView_ListEntryInfo->UpdateDetailsViewInfo(
		CastChecked<UListDataObject_Base>(InSelectedItem),
		TryGetEntryWidgetClassName(InSelectedItem)
	);
}

void UWidget_OptionsScreen::OnListViewListDataModified(UListDataObject_Base* ModifiedData, EOptionListDataModifyReason ModifiedReason)
{
	if (!ModifiedData || bIsResettingData)
	{
		return;
	}

	if (ModifiedData->CanResetBackToDefaultValue())
	{
		ResettableDataArray.AddUnique(ModifiedData);

		if (!GetActionBindings().Contains(ResetActionHandle))
		{
			AddActionBinding(ResetActionHandle);
		}
	}
	else
	{
		if (ResettableDataArray.Contains(ModifiedData))
		{
			ResettableDataArray.Remove(ModifiedData);
		}
	}

	if (ResettableDataArray.IsEmpty())
	{
		RemoveActionBinding(ResetActionHandle);
	}
}
 


FString UWidget_OptionsScreen::TryGetEntryWidgetClassName(UObject* InOwningListItem) const
{
	if (UUserWidget* FoundEntryWidget = CommonListView_OptionsList->GetEntryWidgetFromItem(InOwningListItem))
	{
		return FoundEntryWidget->GetClass()->GetName();
	}
	return TEXT("Entry Widget NOt valid");
}

void UWidget_OptionsScreen::OnResetBoundActionTriggered()
{
	if (ResettableDataArray.IsEmpty())
	{
		return;
	}

	UCommonButtonBase* SelectedTabButton = TabListWidget_OptionsTabs->GetTabButtonBaseByID(TabListWidget_OptionsTabs->GetActiveTab());	

	const FString SelectedTabButtonName = CastChecked<UPIX_CommonButtonBase>(SelectedTabButton)->GetButtonDisplayText().ToString();

	UIX_UISubsystem::Get(this)->PushConfirmScreenToModalStackAsync(
		EConfirmScreenType::YesNo,
		FText::FromString(TEXT("Reset All Settings?")),
		FText::FromString(TEXT("Are you sure you want to reset all the settings under the ") + SelectedTabButtonName + TEXT(" tab?")),
		[this](EConfirmScreenButtonType ClickedButtonType)
		{
			if(ClickedButtonType != EConfirmScreenButtonType::Confirmed)
			{
				return;
			}
			bIsResettingData = true;
			bool bHasDataFailedToReset = false;
			for (UListDataObject_Base* DataToReset : ResettableDataArray)
			{
				if(!DataToReset)
				{
					continue;
				}
				if (DataToReset->TryResetBackToDefaultValue())
				{
					Debug::Print(DataToReset->GetDataDisplayName().ToString() + TEXT(" was reset"));
				}
				else
				{
					bHasDataFailedToReset = true;
					Debug::Print(DataToReset->GetDataDisplayName().ToString() + TEXT(" failed to reset"));
				}

			}
			if (!bHasDataFailedToReset)
			{
				ResettableDataArray.Empty();

				RemoveActionBinding(ResetActionHandle);
			}
			bIsResettingData = false;
		}
	);
}

void UWidget_OptionsScreen::OnBackBoundActionTriggered()
{
	DeactivateWidget();

}
 