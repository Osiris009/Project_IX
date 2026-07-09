// Fill out your copyright notice in the Description page of Project Settings.


#include "IX_UI/Widgets/OptionScreens/ListEntries/Widget_ListEntry_String.h"
#include "IX_UI/Widgets/OptionScreens/DataObjects/ListDataObject_String.h"
#include "IX_UI/Widgets/Components/PIXCommonRotator.h"
#include "IX_UI/Widgets/Components/PIX_CommonButtonBase.h" 

#include "ProjectDebugHelper.h"

void UWidget_ListEntry_String::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CommonButton_PreviousOption->OnClicked().AddUObject(this, &ThisClass::OnPreviousOptionButtonClicked);
	CommonButton_NextOption->OnClicked().AddUObject(this, &ThisClass::OnNextOptionButtonClicked);

	CommonRotator_AvailableOptions->OnClicked().AddLambda([this](){SelectThisListEntryWidget();});
}

void UWidget_ListEntry_String::OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject)
{
	Super::OnOwningListDataObjectSet(InOwningListDataObject); 
	// Since this is Widget_ListEntry_String, we can safely cast the incoming data object to UListDataObject_String and cache it for later use.
	CachedOwningStringDataObject = CastChecked<UListDataObject_String>(InOwningListDataObject);
	
	CommonRotator_AvailableOptions->PopulateTextLabels(CachedOwningStringDataObject->GetAvailableOptionTextArray());
	
	CommonRotator_AvailableOptions->SetSelectiveOptionByText(CachedOwningStringDataObject->GetCurrentDisplayText());
}

void UWidget_ListEntry_String::OnOwningListDataObjectModified(UListDataObject_Base* OwningModifiedData, EOptionListDataModifyReason ModifyReason)
{
	if (CachedOwningStringDataObject)
	{
		CommonRotator_AvailableOptions->SetSelectiveOptionByText(CachedOwningStringDataObject->GetCurrentDisplayText());
	}
}

void UWidget_ListEntry_String::OnPreviousOptionButtonClicked()
{
	
	if (CachedOwningStringDataObject) 
	{
		CachedOwningStringDataObject->BackToPreviousOption();
	}

	SelectThisListEntryWidget();
}

void UWidget_ListEntry_String::OnNextOptionButtonClicked()
{
	
	if (CachedOwningStringDataObject)
	{
		CachedOwningStringDataObject->AdvanceToNextOption();
	}

	SelectThisListEntryWidget();	
}
