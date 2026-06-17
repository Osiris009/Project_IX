// Fill out your copyright notice in the Description page of Project Settings.


#include "IX_UI/Widgets/OptionScreens/OptionDataInterationHelper.h"
#include "IXProjectSettings/PIXGameUserSettings.h"


FOptionDataInterationHelper::FOptionDataInterationHelper(const FString& InStterOrGetterFuncPath)
	:CachedDynamicFunctionPath(InStterOrGetterFuncPath)
{
	CachedWeakGameUserSettings = UPIXGameUserSettings::Get();
}
// Fix class name typo in method definition

FString FOptionDataInterationHelper::GetValueAsString() const
{
	FString OutStringValue;
	// Use PropertyPathHelpers to get the value of the property as a string
	PropertyPathHelpers::GetPropertyValueAsString(
		CachedWeakGameUserSettings.Get(),
		CachedDynamicFunctionPath,
		OutStringValue
	);

	return OutStringValue;
}
void FOptionDataInterationHelper::SetValueFromString(const FString& InStringValue) const
{
	PropertyPathHelpers::SetPropertyValueFromString(
		CachedWeakGameUserSettings.Get(),
		CachedDynamicFunctionPath,
		InStringValue
	);
}
