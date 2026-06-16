// Fill out your copyright notice in the Description page of Project Settings.


#include "IX_UI/Widgets/Components/PIXCommonRotator.h"
#include "CommonTextBlock.h"

void UPIXCommonRotator::SetSelectiveOptionByText(const FText& InTextOption)
{
	// Find the index of the option in the TextLabels array
	const int32 FoundIndex = TextLabels.IndexOfByPredicate(
		[InTextOption](const FText& TextItem)->bool 
		{
			return TextItem.EqualTo(InTextOption);
		}
	);

	if(FoundIndex != INDEX_NONE)
	{
		SetSelectedItem(FoundIndex);
	}
	else
	{
		MyText->SetText(InTextOption);
		UE_LOG(LogTemp, Warning, TEXT("SetSelectiveOptionByText: Option '%s' not found in TextLabels."), *InTextOption.ToString());
	}
}
