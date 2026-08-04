// Fill out your copyright notice in the Description page of Project Settings.


#include "IX_UI/Widgets/OptionScreens/DataObjects/ListDataObject_Scalar.h"
#include "IX_UI/Widgets/OptionScreens/OptionDataInterationHelper.h"


bool UListDataObject_Scalar::CanResetBackToDefaultValue() const
{
	if (HasDefaultValue() && DataDynamicGetter)
	{
		const float DefaultValue = StringToFloat(GetDefaultValueAsString());
		const float CurrentValue = StringToFloat(DataDynamicGetter->GetValueAsString());
		
		return !FMath::IsNearlyEqual(DefaultValue, CurrentValue, 0.01f);
		
	}
	return false;
}

bool UListDataObject_Scalar::TryResetBackToDefaultValue()
{
	if (CanResetBackToDefaultValue())
	{
		if (DataDynamicSetter)
		{
			DataDynamicSetter->SetValueFromString(GetDefaultValueAsString());
			NotifyListDataModified(this, EOptionListDataModifyReason::ResetToDefault);
			return true;
		}
	}
	return false;
}

FCommonNumberFormattingOptions UListDataObject_Scalar::NoDecimal()
{
	FCommonNumberFormattingOptions Options;
	Options.MaximumFractionalDigits = 0;

	return Options;
}

FCommonNumberFormattingOptions UListDataObject_Scalar::WithDecimal(int32 NumFracDigit)
{
	FCommonNumberFormattingOptions Options;
	Options.MaximumFractionalDigits = NumFracDigit;

	return Options;
}

float UListDataObject_Scalar::GetCurrentValue() const
{	
	if (DataDynamicGetter)
	{
		return FMath::GetMappedRangeValueClamped(
			OutputValueRange,
			DisplayValueRange,
			StringToFloat(DataDynamicGetter->GetValueAsString())
		);
	}

	return 0.0f;
}

void UListDataObject_Scalar::SetCurrentValueFromSlider(float InCurrentValue)
{
	if (DataDynamicGetter)
	{
		const float ClampedValue =  FMath::GetMappedRangeValueClamped(
			DisplayValueRange,
			 OutputValueRange,
			  InCurrentValue);
		DataDynamicSetter->SetValueFromString(LexToString(ClampedValue));
		
		NotifyListDataModified(this);
	}
}


float UListDataObject_Scalar::StringToFloat(const FString& InString) const
{	
	float OutConvertedValue = 0.f;

	LexFromString(OutConvertedValue,*InString);

	return OutConvertedValue;
}
