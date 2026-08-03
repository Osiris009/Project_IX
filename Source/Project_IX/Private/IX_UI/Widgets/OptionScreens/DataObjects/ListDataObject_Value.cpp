// Fill out your copyright notice in the Description page of Project Settings.


#include "IX_UI/Widgets/OptionScreens/DataObjects/ListDataObject_Value.h"



void UListDataObject_Value::SetDataDynamicGetter(const TSharedPtr<FOptionDataInterationHelper>& InDynamicGetter)
{
	DataDynamicGetter = InDynamicGetter;
}

 
void UListDataObject_Value::SetDataDynamicSetter(const TSharedPtr<FOptionDataInterationHelper>& InDynamicSetter)
{
	DataDynamicSetter = InDynamicSetter;
}

