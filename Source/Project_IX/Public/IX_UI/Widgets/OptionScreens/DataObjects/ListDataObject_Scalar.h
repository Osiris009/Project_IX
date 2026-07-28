// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IX_UI/Widgets/OptionScreens/DataObjects/ListDataObject_Value.h"
#include "CommonNumericTextBlock.h"

#include "ListDataObject_Scalar.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_IX_API UListDataObject_Scalar : public UListDataObject_Value
{
	GENERATED_BODY()
	
public:	
	LIST_DATA_ACCESSOR(TRange<float>, DisplayValueRange);
	LIST_DATA_ACCESSOR(TRange<float>, OutputValueRange);
	LIST_DATA_ACCESSOR(float, SliderStepSize);
	LIST_DATA_ACCESSOR(ECommonNumericType, DisplayNumericType);
	LIST_DATA_ACCESSOR(FCommonNumberFormattingOptions, NumberFormattingOptions);
	
private:
	
	TRange<float> DisplayValueRange = TRange<float>(0.f,1.f);
	TRange<float> OutputValueRange = TRange<float>(0.f,1.f);
	float SliderStepSize = 0.1f;
	ECommonNumericType DisplayNumericType = ECommonNumericType::Number;
	FCommonNumberFormattingOptions NumberFormattingOptions;
	
};
