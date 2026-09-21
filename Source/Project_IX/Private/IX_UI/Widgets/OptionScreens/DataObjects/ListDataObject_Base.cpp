// Fill out your copyright notice in the Description page of Project Settings.


#include "IX_UI/Widgets/OptionScreens/DataObjects/ListDataObject_Base.h"
#include "IXProjectSettings/PIXGameUserSettings.h"

void UListDataObject_Base::InitDataObject()
{
	OnDataObjectInitialized();
	
}

void UListDataObject_Base::AddEditCondition(const FOptionsDataEditConditionDescriptor& InEditCondition)
{
	EditConditionDescArray.Add(InEditCondition);
}

bool UListDataObject_Base::IsDataCurrentlyEditable()
{
	bool bIsEditable = true;

	if (EditConditionDescArray.IsEmpty())
	{
		return bIsEditable;
	}

	FString CachedDisabledRichReason;

	for (const FOptionsDataEditConditionDescriptor& Condition : EditConditionDescArray)
	{
		if (!Condition.IsValid() || Condition.IsEditConditionMet())
		{
			continue;
		}

		bIsEditable = false;

		CachedDisabledRichReason.Append(Condition.GetDisabledRichReason());

		
		SetDisabledReachText(FText::FromString(CachedDisabledRichReason));

		if (Condition.HasForcedStringValue())
		{
			const FString ForcedStringValue = Condition.GetDisabledForcedStringValue();

			//If the current value this data object has can be set to the forced value
			if (CanSetToForcedStringValue(ForcedStringValue))
			{
				OnSetToForcedStringValue(ForcedStringValue);
			}
		}
	}

	return bIsEditable;
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
