

#include "IX_UI/Widgets/OptionScreens/DataObjects/ListDataObject_String.h"
#include "ProjectDebugHelper.h"
#include "IX_UI/Widgets/OptionScreens/OptionDataInterationHelper.h"




void UListDataObject_String::AddDynamicOption(const FString& InStringValue, const FText InDisplayText)
{
	AvailableOptionStringArray.Add(InStringValue);
	AvailableOptionTextArray.Add(InDisplayText);
}

void UListDataObject_String::AdvanceToNextOption()
{
	if (AvailableOptionStringArray.IsEmpty() || AvailableOptionTextArray.IsEmpty())
	{
		
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

	if(DataDynamicSetter)
	{
		DataDynamicSetter->SetValueFromString(CurrentStringValue);
		
		NotifyListDataModified(this);
	}

}

void UListDataObject_String::BackToPreviousOption()
{
	if (AvailableOptionStringArray.IsEmpty() || AvailableOptionTextArray.IsEmpty())
	{
		
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
	if (DataDynamicSetter)
	{
		DataDynamicSetter->SetValueFromString(CurrentStringValue);
		
		NotifyListDataModified(this);
	}
}

void UListDataObject_String::OnRotatorInitiatedValueChanged(const FText& InNewSelectedText)
{
	const int32 FoundIndex = AvailableOptionTextArray.IndexOfByPredicate(
		[InNewSelectedText](const FText& AvailableText)->bool
		{
			return AvailableText.EqualTo(InNewSelectedText);
		}
	);

	if(FoundIndex!= INDEX_NONE && AvailableOptionStringArray.IsValidIndex(FoundIndex))
	{
		CurrentDisplayText = InNewSelectedText;
		CurrentStringValue = AvailableOptionStringArray[FoundIndex];

		if (DataDynamicSetter)
		{
			DataDynamicSetter->SetValueFromString(CurrentStringValue);
			
			NotifyListDataModified(this);
		} 
	}
	

}

void UListDataObject_String::OnDataObjectInitialized()
{
	if (!AvailableOptionStringArray.IsEmpty())
	{
		CurrentStringValue = AvailableOptionStringArray[0];
		//Debug::Print(FString::Printf(TEXT("UListDataObject_String::OnDataObjectInitialized: CurrentStringValue set to %s"), *CurrentStringValue), 5.f, FColor::Green);
	}
	
	//TODO :: Read from the saved string value and use it to set the CurrentStringValue  
	if (DataDynamicGetter)
	{
		if (!DataDynamicGetter->GetValueAsString().IsEmpty())
		{
			CurrentStringValue = DataDynamicGetter->GetValueAsString();
			
		}
		
	}

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

bool UListDataObject_String::CanResetBackToDefaultValue() const
{
	return HasDefaultValue() && CurrentStringValue != GetDefaultValueAsString();
}

bool UListDataObject_String::TryResetBackToDefaultValue()
{
	if(CanResetBackToDefaultValue())
	{
		CurrentStringValue = GetDefaultValueAsString();

		TrySetDisplayTextFromStringValue(CurrentStringValue);

		if (DataDynamicSetter)
		{
			DataDynamicSetter->SetValueFromString(CurrentStringValue);
			
			NotifyListDataModified(this, EOptionListDataModifyReason::ResetToDefault);

			return true;
		}
	}

	return false;
}

/*** ============================ 
***UListDataObject_StringBool***
=================================***/

void UListDataObject_StringBool::OverrideTrueDisplayText(const FText& InDisplayText)
{
	if (!AvailableOptionStringArray.Contains(TrueString))
	{
		AddDynamicOption(TrueString,InDisplayText);
	}
}

void UListDataObject_StringBool::OverrideFalseDisplayText(const FText& InDisplayText)
{
	if (!AvailableOptionStringArray.Contains(FalseString))
	{
		AddDynamicOption(FalseString,InDisplayText);
	}
}

void UListDataObject_StringBool::SetTrueAsDefaultValue()
{
	SetDefaultValueFromString(TrueString);
}

void UListDataObject_StringBool::SetFalseAsDefaultValue()
{
	SetDefaultValueFromString(FalseString);
}

void UListDataObject_StringBool::OnDataObjectInitialized()
{
	TryInitBoolValues();
	Super::OnDataObjectInitialized();
}

void UListDataObject_StringBool::TryInitBoolValues()
{
	if (!AvailableOptionStringArray.Contains(TrueString))
	{
		AddDynamicOption(TrueString,FText::FromString(TEXT("On")));
	}
	if (!AvailableOptionStringArray.Contains(FalseString))
	{
		AddDynamicOption(FalseString,FText::FromString(TEXT("Off")));
	}
}

