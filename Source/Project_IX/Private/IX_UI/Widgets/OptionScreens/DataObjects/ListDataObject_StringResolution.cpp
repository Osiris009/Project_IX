// Fill out your copyright notice in the Description page of Project Settings.


#include "IX_UI/Widgets/OptionScreens/DataObjects/ListDataObject_StringResolution.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ProjectDebugHelper.h"
#include "IX_UI/Widgets/OptionScreens/OptionDataInterationHelper.h"
#include "IXProjectSettings/PIXGameUserSettings.h"

void UListDataObject_StringResolution::InitResolutionValues()
{
	TArray<FIntPoint> AvailableResolutions;
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(AvailableResolutions);
	
	AvailableResolutions.Sort(
		[](const FIntPoint& A, const FIntPoint& B)->bool
		{
			return A.SizeSquared()<B.SizeSquared();
		}
	);
	
	for (const FIntPoint Resolution : AvailableResolutions)
	{
		//Debug::Print(TEXT("Avaialble Resolution: ") + Resolution.ToString());
		AddDynamicOption(ResToValueString(Resolution), ResToDisplayText(Resolution));
	}
	
	MaximumAllowedResolution = ResToValueString(AvailableResolutions.Last());
	
	SetDefaultValueFromString(MaximumAllowedResolution);
	
}

void UListDataObject_StringResolution::OnDataObjectInitialized()
{
	
	Super::OnDataObjectInitialized();
	
	if(!TrySetDisplayTextFromStringValue(MaximumAllowedResolution))
	{
		//CurrentDisplayText
		CurrentDisplayText = ResToDisplayText(UPIXGameUserSettings::Get()->GetScreenResolution());
	}
	
} 

FString UListDataObject_StringResolution::ResToValueString(const FIntPoint& InResolution) const
{
	return FString::Printf(TEXT("(X=%i, Y=%i)"), InResolution.X, InResolution.Y);
	
}

FText UListDataObject_StringResolution::ResToDisplayText(const FIntPoint& InResolution) const
{
	const FString DisplayString = FString::Printf(TEXT("%i x %i"), InResolution.X, InResolution.Y);
	return FText::FromString(DisplayString);
}
