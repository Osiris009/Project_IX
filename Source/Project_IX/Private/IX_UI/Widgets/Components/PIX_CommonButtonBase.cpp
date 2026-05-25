// Fill out your copyright notice in the Description page of Project Settings.


#include "IX_UI/Widgets/Components/PIX_CommonButtonBase.h"
#include "CommonTextBlock.h"
#include "IX_UI/IX_UISubsystem.h"

void UPIX_CommonButtonBase::SetButtonText(FText InButtonText)
{
	if (CommonnTextBlock_ButtonText && !InButtonText.IsEmpty())
	{
		CommonnTextBlock_ButtonText->SetText(bUserUpperCaseForButtonText ? InButtonText.ToUpper() : InButtonText);
	}
}

void UPIX_CommonButtonBase::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetButtonText(ButtonDisplayText);

}

void UPIX_CommonButtonBase::NativeOnCurrentTextStyleChanged()
{
	Super::NativeOnCurrentTextStyleChanged();
	if (CommonnTextBlock_ButtonText && GetCurrentTextStyleClass())
	{
		CommonnTextBlock_ButtonText->SetStyle(GetCurrentTextStyleClass());
	}
}

void UPIX_CommonButtonBase::NativeOnHovered()
{
	Super::NativeOnHovered();

	if (!ButtonDiscriptionText.IsEmpty())
	{
		UIX_UISubsystem::Get(this)->OnButtonDescriptionTextUpdated.Broadcast(this, ButtonDiscriptionText);
	}
}

void UPIX_CommonButtonBase::NativeOnUnhovered()
{
	Super::NativeOnUnhovered();

	UIX_UISubsystem::Get(this)->OnButtonDescriptionTextUpdated.Broadcast(this, FText::GetEmpty());
}


