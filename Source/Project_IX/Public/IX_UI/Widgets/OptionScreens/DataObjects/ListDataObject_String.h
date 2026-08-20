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
	// The function will try to find the InStringValue in the AvailableOptionStringArray, 
	// if found, it will set the CurrentDisplayText to the corresponding text 
	// in AvailableOptionTextArray and return true. If not found, it will return false and do nothing.
	void AdvanceToNextOption();
	void BackToPreviousOption();
	
	void OnRotatorInitiatedValueChanged(const FText& InNewSelectedText);

protected:

	//~Form UListDataObject base
	virtual void OnDataObjectInitialized() override;
	
	bool TrySetDisplayTextFromStringValue(const FString& InStringValue);
	FString CurrentStringValue;
	FText CurrentDisplayText;
	
	TArray<FString> AvailableOptionStringArray;
	TArray<FText> AvailableOptionTextArray;

	virtual bool CanResetBackToDefaultValue() const override;
	virtual bool TryResetBackToDefaultValue() override;
public:
	//~Form UListDataObject base 
	FORCEINLINE const TArray<FText>& GetAvailableOptionTextArray() const { return AvailableOptionTextArray;}
	
	FORCEINLINE FText GetCurrentDisplayText() const { return CurrentDisplayText; }
};

UCLASS()
class PROJECT_IX_API UListDataObject_StringBool : public UListDataObject_String
{
	GENERATED_BODY()
	
public:
	
	void OverrideTrueDisplayText(const FText& InDisplayText);
	void OverrideFalseDisplayText(const FText& InDisplayText);
	
	void SetTrueAsDefaultValue();
	void SetFalseAsDefaultValue();
	
	
protected:
	
	virtual void OnDataObjectInitialized() override;
	
private:
	void TryInitBoolValues();
	
	const FString TrueString = "true";
	const FString FalseString = "false";
};

UCLASS()
class PROJECT_IX_API UListDataObject_StringEnum : public UListDataObject_String
{
	GENERATED_BODY()
	
public:
	
	template<typename EnumType>
	void AddEnumOption(EnumType InEnumOption, const FText& InDisplayText)
	{
		const UEnum* StaticEnumOption = StaticEnum<EnumType>();
		const FString ConvertedEnumString = StaticEnumOption->GetNameStringByValue(InEnumOption);

		AddDynamicOption(ConvertedEnumString,InDisplayText);
	}

	template<typename EnumType>
	EnumType GetCurrentValueAsEnum() const
	{
		const UEnum* StaticEnumOption = StaticEnum<EnumType>();

		return (EnumType)StaticEnumOption->GetValueByNameString(CurrentStringValue);
	}

	template<typename EnumType>
	void SetDefaultValueFromEnumOption(EnumType InEnumOption)
	{
		const UEnum* StaticEnumOption = StaticEnum<EnumType>();
		const FString ConvertedEnumString = StaticEnumOption->GetNameStringByValue(InEnumOption);

		SetDefaultValueFromString(ConvertedEnumString);
	}
	
};
