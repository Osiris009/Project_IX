// Fill out your copyright notice in the Description page of Project Settings.


#include "IX_UI/Widgets/Components/ProIXTabListWidgetBase.h"
#include "Editor/WidgetCompilerLog.h"
#include "IX_UI/Widgets/Components/PIX_CommonButtonBase.h"

void UProIXTabListWidgetBase::RequestRegisterTab(const FName& InTabID, const FText& InTabDisplayName)
{
	RegisterTab(InTabID, TabButtonEntryWidgetClass, nullptr);

	if (UPIX_CommonButtonBase* FoundButton = Cast<UPIX_CommonButtonBase>(GetTabButtonBaseByID(InTabID)))
	{
		FoundButton->SetButtonText(InTabDisplayName);

	}
}

#if WITH_EDITOR
void UProIXTabListWidgetBase::ValidateCompiledDefaults(IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledDefaults(CompileLog);

	if (!TabButtonEntryWidgetClass)
	{
		CompileLog.Error(FText::FromString(
			TEXT("The variable TabButtonEntryWidgetClass has no valid entry specified. ") +
			GetClass()->GetName() +
			TEXT(" needs a valid entry widget class to function properly")
		));
	}
}
#endif