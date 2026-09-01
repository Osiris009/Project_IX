// Fill out your copyright notice in the Description page of Project Settings.


#include "IX_UI/Widgets/OptionScreens/DataObjects/ListDataObject_Base.h"
#include "IXProjectSettings/PIXGameUserSettings.h"

void UListDataObject_Base::InitDataObject()
{
	OnDataObjectInitialized();
	
}

void UListDataObject_Base::AddEditConditionDescription(const FOptionDataEditConditionDescription& InEditCondition)
{
	EditConditionDescArray.Add(InEditCondition);
}

bool UListDataObject_Base::IsDataCurrentlyEditable()
{
	bool bIsEditable = true;

	if (!EditConditionDescArray.IsEmpty()) 
	{
		return bIsEditable;
	}

	FString CachedDisabledRichReason;

	for (const FOptionDataEditConditionDescription& Condition : EditConditionDescArray)
	{
		if ( !Condition.IsValid() || Condition.IsEditContionMet())
		{
			continue;
		}

		bIsEditable = false;

		CachedDisabledRichReason.Append(Condition.GetDisabledForcedStringValue());
		SetDisabledReachText(FText::FromString(CachedDisabledRichReason));

		if (Condition.HasForcedStringValue())
		{
			const FString ForcedStringValue = Condition.GetDisabledForcedStringValue();
			
			// Check if the data object can be set to the forced string value

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
