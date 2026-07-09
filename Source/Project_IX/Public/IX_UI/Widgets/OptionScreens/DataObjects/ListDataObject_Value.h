// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IX_UI/Widgets/OptionScreens/DataObjects/ListDataObject_Base.h"
#include "ListDataObject_Value.generated.h"


class FOptionDataInterationHelper;

UCLASS(Abstract)
class PROJECT_IX_API UListDataObject_Value : public UListDataObject_Base
{
	GENERATED_BODY()
	
public:

	void SetDtataDynamicGetter(const TSharedPtr<FOptionDataInterationHelper>& InDynamicGetter);

	void SetDtataDynamicSetter(const TSharedPtr<FOptionDataInterationHelper>& InDynamicSetter);

	
	void SetDefaultValueFromString(const FString& InDefaultValue){
		DefaultStringValue = InDefaultValue;
	}
	// The child class should override this function to return true if the data object has a default value.
	virtual bool HasDefaultValue() const override { return DefaultStringValue.IsSet(); }

protected:

	// Called when the data object is initialized. Child classes can override this to perform additional initialization.
	TSharedPtr<FOptionDataInterationHelper> DataDynamicGetter;
	TSharedPtr<FOptionDataInterationHelper> DataDynamicSetter; 

	FString GetDefaultValueAsString() const { return DefaultStringValue.GetValue(); }

private:

	TOptional<FString> DefaultStringValue;

};
