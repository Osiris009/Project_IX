// Fill out your copyright notice in the Description page of Project Settings.


#include "IX_UI/Widgets/OptionScreens/DataObjects/ListDataObject_String.h"
#include "ProjectDebugHelper.h"

void UListDataObject_String::AddDynamicOption(const FString& InStringValue, const FText InDisplayText)
{
	AvailableOptionStringArray.Add(InStringValue);
	AvailableOptionTextArray.Add(InDisplayText);
}

void UListDataObject_String::AdvanceToNextOption()
{
	if (AvailableOptionStringArray.IsEmpty() || AvailableOptionTextArray.IsEmpty())
	{
		Debug::Print(TEXT("UListDataObject_String::AdvanceToNextOption: No available options to advance to."), 5.f, FColor::Red);
		return;
	}

	const int32 CurrentIndex = AvailableOptionStringArray.IndexOfByKey(CurrentStringValue);
	const int32 NextIndexToDisplay = CurrentIndex + 1 ;

	const bool bIsValidNextIndex = AvailableOptionStringArray.IsValidIndex(NextIndexToDisplay);

	if(bIsValidNextIndex)
	{
		CurrentStringValue = AvailableOptionStringArray[NextIndexToDisplay];
	}
	else
	{
		CurrentStringValue = AvailableOptionStringArray[0];	
	}

	TrySetDisplayTextFromStringValue(CurrentStringValue);
	NotifyListDataModified(this);
}

void UListDataObject_String::BackToPreviousOption()
{
	if (AvailableOptionStringArray.IsEmpty() || AvailableOptionTextArray.IsEmpty())
	{
		Debug::Print(TEXT("UListDataObject_String::AdvanceToNextOption: No available options to advance to."), 5.f, FColor::Red);
		return;
	}

	const int32 CurrentIndex = AvailableOptionStringArray.IndexOfByKey(CurrentStringValue);
	const int32 PreviousIndexToDisplay = CurrentIndex - 1;

	const bool bIsValidPreviousIndex = AvailableOptionStringArray.IsValidIndex(PreviousIndexToDisplay);

	if (bIsValidPreviousIndex)
	{
		CurrentStringValue = AvailableOptionStringArray[PreviousIndexToDisplay];
	}
	else
	{
		CurrentStringValue = AvailableOptionStringArray.Last();
	}

	TrySetDisplayTextFromStringValue(CurrentStringValue);
	NotifyListDataModified(this);
}

void UListDataObject_String::OnDataObjectInitialized()
{
	if (!AvailableOptionStringArray.IsEmpty())
	{
		CurrentStringValue = AvailableOptionStringArray[0];
		Debug::Print(FString::Printf(TEXT("UListDataObject_String::OnDataObjectInitialized: CurrentStringValue set to %s"), *CurrentStringValue), 5.f, FColor::Green);
	}
	
	//TODO :: Read from the saved string value and use it to set the CurrentStringValue  
	
	if(!TrySetDisplayTextFromStringValue(CurrentStringValue))
	{
		CurrentDisplayText = FText::FromString(TEXT("InValidOption"));
	}
	
}

bool UListDataObject_String::TrySetDisplayTextFromStringValue(const FString& InStringValue)
{
	const int32 CurrentFoundIndex = AvailableOptionStringArray.IndexOfByKey(InStringValue);
	
	if (AvailableOptionTextArray.IsValidIndex(CurrentFoundIndex))
	{
		CurrentDisplayText = AvailableOptionTextArray[CurrentFoundIndex];
		return true;
	}	
	return false ;
}

