// Fill out your copyright notice in the Description page of Project Settings.


#include "IX_UI/Widgets/OptionScreens/Widget_OptionsDetailsView.h"
#include "CommonTextBlock.h"
#include "CommonLazyImage.h"
#include "CommonRichTextBlock.h"	
#include "IX_UI/Widgets/OptionScreens/DataObjects/ListDataObject_Base.h"


void UWidget_OptionsDetailsView::UpdateDetailsViewInfo(UListDataObject_Base* InDataObject, const FString& InEntryWidgetClassName)
{
	if (!InDataObject)
	{
		return;
	}

	CommonTextBlock_Titel->SetText(InDataObject->GetDataDisplayName());

	if(!InDataObject->GetSoftDescriptionImage().IsNull())
	{
		CommonLazyImage_DiscriptionImage->SetBrushFromSoftTexture(InDataObject->GetSoftDescriptionImage());
		CommonLazyImage_DiscriptionImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	
	CommonRichText_Description->SetText(InDataObject->GetDescriptionRichText());

	const FString DynamicDetails = FString::Printf(
		TEXT("Data Object Class: <Bold>%s</>\n\nEntry Widget Class:<Bold>%s</>"),
		*InDataObject->GetClass()->GetName(),
		*InEntryWidgetClassName
	);
	CommonRichText_DynamicDetails->SetText(FText::FromString(DynamicDetails));

	CommonRichText_DisabledResons->SetText(InDataObject->GetDisabledReachText());

}

void UWidget_OptionsDetailsView::ClearDetailsViewInfo()
{
	// Clear the details view information by resetting the text and image to empty or null values
	CommonTextBlock_Titel->SetText(FText::GetEmpty());
	CommonLazyImage_DiscriptionImage->SetVisibility(ESlateVisibility::Collapsed);
	CommonRichText_Description->SetText(FText::GetEmpty());
	CommonRichText_DynamicDetails->SetText(FText::GetEmpty());
	CommonRichText_DisabledResons->SetText(FText::GetEmpty());
}

void UWidget_OptionsDetailsView::NativeOnInitialized()
{

	Super::NativeOnInitialized();
	ClearDetailsViewInfo();

}
              