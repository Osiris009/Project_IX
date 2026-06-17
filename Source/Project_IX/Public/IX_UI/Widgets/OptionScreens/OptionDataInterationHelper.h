// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PropertyPathHelpers.h"

class UPIXGameUserSettings;

class PROJECT_IX_API FOptionDataInterationHelper
{
public:

	FOptionDataInterationHelper(const FString& InStterOrGetterFuncPath);
	
	FString GetValueAsString() const;
	void SetValueFromString(const FString& InStringValue) const;

private:	
	/*Using this to store the path to the getter /setter function of the option data, 
	so we can use it to get/set the value of the option data without
	having to use reflection every time.*/
	FCachedPropertyPath CachedDynamicFunctionPath;

	//In native C++ we can't use UPROPERTY macro above a raw pointer , 
	// so we use TWeakObjectPtr to store a weak reference to the UPIXGameUserSettings object.
	TWeakObjectPtr<UPIXGameUserSettings> CachedWeakGameUserSettings;

	 
};
