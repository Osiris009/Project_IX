

#include "IX_UI/Widgets/OptionScreens/DataAsset_DataListEntryMapping.h"
#include "IX_UI/Widgets/OptionScreens/DataObjects/ListDataObject_Base.h"

TSubclassOf<UWidget_ListEntry_Base> UDataAsset_DataListEntryMapping::FindEntryWidgetClassByDataObject(
	UListDataObject_Base* InDataObject) const
{
	check(InDataObject != nullptr);
	
	//
	for (UClass* DataObjectClass = InDataObject->GetClass();DataObjectClass ; DataObjectClass = DataObjectClass->GetSuperClass() )
	{
		if(TSubclassOf<UListDataObject_Base> ConvertedDataObjectClass = TSubclassOf<UListDataObject_Base>(DataObjectClass))
		{
			if (DataObjectListEntryMap.Contains(ConvertedDataObjectClass))
			{
				return DataObjectListEntryMap.FindRef(ConvertedDataObjectClass);
			}
		}
	}
	
	return TSubclassOf<UWidget_ListEntry_Base>();
	
}
