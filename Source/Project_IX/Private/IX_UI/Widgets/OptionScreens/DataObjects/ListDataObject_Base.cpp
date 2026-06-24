// Fill out your copyright notice in the Description page of Project Settings.


#include "IX_UI/Widgets/OptionScreens/DataObjects/ListDataObject_Base.h"
#include "IXProjectSettings/PIXGameUserSettings.h"

void UListDataObject_Base::InitDataObject()
{
	OnDataObjectInitialized();
	
}

void UListDataObject_Base::OnDataObjectInitialized()
{
}

void UListDataObject_Base::NotifyListDataModified(UListDataObject_Base* ModifiedData, 
	EOptionListDataModifyReason ModifyReason)
{
	OnListDataModified.Broadcast(ModifiedData, ModifyReason);
	if (bShouldApplyChangeImimediately)
	{
		UPIXGameUserSettings::Get()->ApplySettings(true);
	}
}
