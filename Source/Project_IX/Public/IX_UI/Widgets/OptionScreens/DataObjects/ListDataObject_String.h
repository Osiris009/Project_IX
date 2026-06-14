// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "IX_UI/Widgets/OptionScreens/DataObjects/ListDataObject_Value.h"
#include "ListDataObject_String.generated.h"


UCLASS()
class PROJECT_IX_API UListDataObject_String : public UListDataObject_Value
{
	GENERATED_BODY()
	
public:	
	void AddDynamicOption(const FString& InStringValue, const FText InDisplayText);
	
protected:
	
	FString CurrentStringValue;
	FText CurrentDisplayText;
	
	//~Form UListDataObject base
	virtual void OnDataObjectInitialized() override;
	
	bool TrySetDisplayTextFromStringValue(const FString& InStringValue);
	
	
	TArray<FString> AvailableOptionStringArray;
	TArray<FText> AvailableOptionTextArray;
};
