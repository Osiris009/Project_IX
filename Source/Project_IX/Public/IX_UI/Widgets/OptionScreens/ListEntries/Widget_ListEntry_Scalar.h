// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IX_UI/Widgets/OptionScreens/ListEntries/Widget_ListEntry_Base.h"
#include "Widget_ListEntry_Scalar.generated.h"

class UCommonNumericTextBlock;
class UAnalogSlider;

UCLASS(Abstract, BlueprintType, meta = (DisableNaiveTick))
class PROJECT_IX_API UWidget_ListEntry_Scalar : public UWidget_ListEntry_Base
{
	GENERATED_BODY()
	
protected:
	
	//~ Begin UWidget interface
	virtual void NativeOnInitialized() override;
	
	virtual void OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject) override;
	
	virtual void OnOwningListDataObjectModified(UListDataObject_Base* OwningModifiedData, EOptionListDataModifyReason ModifyReason) override;
	
	
private:	
	//*** bind widget 
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = "true"))
	UCommonNumericTextBlock* CommonNumeric_SettingValue;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = "true"))
	UAnalogSlider* CommonSlider_SettingSlider;
	
};
