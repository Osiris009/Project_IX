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
        return Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable);
    }

    TSubclassOf<UWidget_ListEntry_Base> FoundWidgetClass = DataListEntryMapping->FindEntryWidgetClassByDataObject(CastChecked<UListDataObject_Base>(Item));

	return GenerateTypedEntry<UWidget_ListEntry_Base>(FoundWidgetClass, OwnerTable);

}

#if WITH_EDITOR	
void UPIX_CommonListView::ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const
{
    Super::ValidateCompiledDefaults(CompileLog);
    
    if (!DataListEntryMapping)
    {
        CompileLog.Error(FText::FromString(("DataListEntryMapping: NULL") + GetClass()->GetName() + TEXT("needs a valid data asset to function properly")));
        
    }
   
    UCommonListView::ValidateCompiledDefaults(CompileLog);
}
#endif