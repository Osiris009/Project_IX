// Fill out your copyright notice in the Description page of Project Settings.


#include "IX_UI/Widgets/OptionScreens/DataObjects/ListDataObject_String.h"

void UListDataObject_String::AddDynamicOption(const FString& InStringValue, const FText InDisplayText)
{
	AvailableOptionStringArray.Add(InStringValue);
	AvailableOptionTextArray.Add(InDisplayText);
}

void UListDataObject_String::OnDataObjectInitialized()
{
	if (!AvailableOptionStringArray.IsEmpty())
	{
		CurrentStringValue = AvailableOptionStringArray[0];
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

