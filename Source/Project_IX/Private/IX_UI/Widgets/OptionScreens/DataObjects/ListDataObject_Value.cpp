// Fill out your copyright notice in the Description page of Project Settings.


#include "IX_UI/Widgets/OptionScreens/DataObjects/ListDataObject_Value.h"



void UListDataObject_Value::SetDtataDynamicGetter(const TSharedPtr<FOptionDataInterationHelper>& InDynamicGetter)
{
	DataDynamicGetter = InDynamicGetter;
}

 
void UListDataObject_Value::SetDtataDynamicSetter(const TSharedPtr<FOptionDataInterationHelper>& InDynamicSetter)
{
	DataDynamicSetter = InDynamicSetter;
}

