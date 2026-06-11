// Fill out your copyright notice in the Description page of Project Settings.


#include "IX_UI/Widgets/Components/PIX_CommonListView.h"
#include "Editor/WidgetCompilerLog.h"
#include "IX_UI/Widgets/OptionScreens/DataAsset_DataListEntryMapping.h"
#include "IX_UI/Widgets/OptionScreens/ListEntries/Widget_ListEntry_Base.h"
#include "IX_UI/Widgets/OptionScreens/DataObjects/ListDataObject_Base.h"



UUserWidget& UPIX_CommonListView::OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable)
{
    // TODO: insert return statement here
    if (IsDesignTime())
    {
		// During design time, we can't rely on the DataListEntryMapping being properly set up, so we'll just return a default widget
        return Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable);
    }

	// Check if the DataListEntryMapping is valid before trying to use it
    if (TSubclassOf<UWidget_ListEntry_Base> FoundWidgetClass = DataListEntryMapping->FindEntryWidgetClassByDataObject(CastChecked<UListDataObject_Base>(Item)))
    {
        return GenerateTypedEntry<UWidget_ListEntry_Base>(FoundWidgetClass, OwnerTable);
    }

    else
    {
		//UE_LOG(LogTemp, Warning, TEXT("UPIX_CommonListView::OnGenerateEntryWidgetInternal - No widget class found for data object: %s"), *GetNameSafe(Item));
        return Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable);
    }

}

//~~ UWidget interface
#if WITH_EDITOR	
void UPIX_CommonListView::ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const
{
    Super::ValidateCompiledDefaults(CompileLog);
    
	// Ensure that the DataListEntryMapping is set up properly, as it's critical for the functionality of this widget
    if (!DataListEntryMapping)
    {
        CompileLog.Error(FText::FromString(("DataListEntryMapping: NULL") + GetClass()->GetName() + TEXT("needs a valid data asset to function properly")));
        
    }
   
    UCommonListView::ValidateCompiledDefaults(CompileLog);
}
#endif                            