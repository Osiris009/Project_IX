// Fill out your copyright notice in the Description page of Project Settings.


#include "IX_UI/Widgets/Widget_ConfirmScreen.h"
#include "CommonTextBlock.h"
#include "Components/DynamicEntryBox.h"
#include "IX_UI/Widgets/Components/PIX_CommonButtonBase.h"
#include "ICommonInputModule.h" // For ECommonInputType
#include "Engine/DataTable.h"


UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateOkScreen(const FText& InScreenTitle, const FText& InScreenMsg)
{
	UConfirmScreenInfoObject* InfoObject = NewObject<UConfirmScreenInfoObject>();
	InfoObject->ScreenTitle = InScreenTitle;
	InfoObject->ScreenMessage = InScreenMsg;
	FConfirmScreenButtonInfo OKButtonInfo;
	OKButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Closed;
	OKButtonInfo.ButtonTextToDisplay = FText::FromString(TEXT("OK"));
	 
	InfoObject->AvailableScreenButtons.Add(OKButtonInfo);

	return InfoObject;
}


UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateYesNoScreen(const FText& InScreenTitle, const FText& InScreenMsg)
{
	UConfirmScreenInfoObject* InfoObject = NewObject<UConfirmScreenInfoObject>();
	InfoObject->ScreenTitle = InScreenTitle;
	InfoObject->ScreenMessage = InScreenMsg;

	FConfirmScreenButtonInfo YesButtonInfo;
	YesButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Confirmed;
	YesButtonInfo.ButtonTextToDisplay = FText::FromString(TEXT("Yes"));

	FConfirmScreenButtonInfo NoButtonInfo;
	NoButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Cancelled;
	NoButtonInfo.ButtonTextToDisplay = FText::FromString(TEXT("No"));

	InfoObject->AvailableScreenButtons.Add(YesButtonInfo);
	InfoObject->AvailableScreenButtons.Add(NoButtonInfo);

	return InfoObject;
}

UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateOkCancelScreen(const FText& InScreenTitle, const FText& InScreenMsg)
{
	UConfirmScreenInfoObject* InfoObject = NewObject<UConfirmScreenInfoObject>();
	InfoObject->ScreenTitle = InScreenTitle;
	InfoObject->ScreenMessage = InScreenMsg;

	FConfirmScreenButtonInfo OkButtonInfo;
	OkButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Confirmed;
	OkButtonInfo.ButtonTextToDisplay = FText::FromString(TEXT("Ok"));

	FConfirmScreenButtonInfo CancelButtonInfo;
	CancelButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Cancelled;
	CancelButtonInfo.ButtonTextToDisplay = FText::FromString(TEXT("Cancel"));

	InfoObject->AvailableScreenButtons.Add(OkButtonInfo);
	InfoObject->AvailableScreenButtons.Add(CancelButtonInfo);

	return InfoObject;
}

UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateYesOkCancelScreen(const FText& InScreenTitle, const FText& InScreenMsg)
{
	UConfirmScreenInfoObject* InfoObject = NewObject<UConfirmScreenInfoObject>();
	InfoObject->ScreenTitle = InScreenTitle;
	InfoObject->ScreenMessage = InScreenMsg;

	FConfirmScreenButtonInfo YesButtonInfo;
	YesButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Confirmed;
	YesButtonInfo.ButtonTextToDisplay = FText::FromString(TEXT("Yes"));

	FConfirmScreenButtonInfo OkButtonInfo;
	OkButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Confirmed;
	OkButtonInfo.ButtonTextToDisplay = FText::FromString(TEXT("Ok"));

	FConfirmScreenButtonInfo CancelButtonInfo;
	CancelButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Cancelled;
	CancelButtonInfo.ButtonTextToDisplay = FText::FromString(TEXT("Cancel"));
	
	InfoObject->AvailableScreenButtons.Add(YesButtonInfo);
	InfoObject->AvailableScreenButtons.Add(OkButtonInfo);
	InfoObject->AvailableScreenButtons.Add(CancelButtonInfo);

	return InfoObject;
}

void UWidget_ConfirmScreen::InitConfirmScreen(UConfirmScreenInfoObject* InConfirmScreenInfo,
	TFunction<void(EConfirmScreenButtonType)> ClickedButtonCallback)
{
	check(InConfirmScreenInfo && CommonTextBlock_Tital && CommonTextBlock_Message && DynamicEntryBox_Buttons);

	CommonTextBlock_Tital->SetText(InConfirmScreenInfo->ScreenTitle);
	CommonTextBlock_Message->SetText(InConfirmScreenInfo->ScreenMessage);


	// Clear any existing buttons in the entry box before adding new ones\
	// This ensures that if the confirm screen is reused, it won't have leftover buttons from previous uses.
	if (DynamicEntryBox_Buttons->GetNumEntries() != 0 )
	{
		/*
		*	Clearing the old buttons the entry box has. The widget type for the entry box
		*	is specifled in the child widget blueprint.
		*/
		DynamicEntryBox_Buttons->Reset<UPIX_CommonButtonBase>(
			[ClickedButtonCallback](UPIX_CommonButtonBase& ExistingButton)
			{
				ExistingButton.OnClicked().Clear();
			}
		);

	}

	//check(!InConfirmScreenInfo->AvailableScreenButtons.IsEmpty());

	for(const FConfirmScreenButtonInfo& AvailableButtonInfo : InConfirmScreenInfo->AvailableScreenButtons)
	{

		FDataTableRowHandle InputActionRowHandle;

		switch (AvailableButtonInfo.ConfirmScreenButtonType)
		{
		case EConfirmScreenButtonType::Confirmed:
			InputActionRowHandle = ICommonInputModule::GetSettings().GetDefaultClickAction();
			break;

		case EConfirmScreenButtonType::Cancelled:
			InputActionRowHandle = ICommonInputModule::GetSettings().GetDefaultBackAction();
			break;

		case EConfirmScreenButtonType::Closed:
			InputActionRowHandle = ICommonInputModule::GetSettings().GetDefaultBackAction();
			break;

		default:
			break;
		}


		UPIX_CommonButtonBase* AdddeButton = DynamicEntryBox_Buttons->CreateEntry<UPIX_CommonButtonBase>();
		AdddeButton->SetButtonText(AvailableButtonInfo.ButtonTextToDisplay);
		
		AdddeButton->SetTriggeringInputAction(InputActionRowHandle);
		// Binding the click event of the button to the provided callback, passing the button type as an argument.
		AdddeButton->OnClicked().AddLambda(
			[ClickedButtonCallback, AvailableButtonInfo,this]()
			{
				// When the button is clicked, this lambda will be executed,
				// which in turn calls the ClickedButtonCallback with the specific button type that was clicked.
				ClickedButtonCallback(AvailableButtonInfo.ConfirmScreenButtonType);
								
				DeactivateWidget(); 
				// Deactivating the confirm screen widget after a button is clicked.
				// This assumes that the desired behavior is to close the confirm screen once a selection is made. 
				// If this is not the intended behavior, this line can be removed or modified as needed.
			}
		);
	}

	if (DynamicEntryBox_Buttons->GetNumEntries() != 0)
	{
		/*
		*	Set focus on the last button. So if there are two buttons, one is yes, one is no.
		*	Our gamepad will focus on the No button
		*/
		DynamicEntryBox_Buttons->GetAllEntries().Last()->SetFocus();
	}

}
 

