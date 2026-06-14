// Fill out your copyright notice in the Description page of Project Settings.


#include "IX_UI/Widgets/OptionScreens/ListEntries/Widget_ListEntry_String.h"
#include "IX_UI/Widgets/OptionScreens/DataObjects/ListDataObject_String.h"



void UWidget_ListEntry_String::OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject)
{
	Super::OnOwningListDataObjectSet(InOwningListDataObject); 
	
	CachedOwningStringDataObject = CastChecked<UListDataObject_String>(InOwningListDataObject);
	
	
	
}
