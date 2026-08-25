// Fill out your copyright notice in the Description page of Project Settings.


#include "IX_UI/Widgets/OptionScreens/DataObjects/ListDataObject_StringResolution.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ProjectDebugHelper.h"
#include "IX_UI/Widgets/OptionScreens/OptionDataInterationHelper.h"


void UListDataObject_StringResolution::InitResolutionValues()
{
	TArray<FIntPoint> AvailableResolutions;
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(AvailableResolutions);
	
	for (const FIntPoint Resolution : AvailableResolutions)
	{
		Debug::Print(TEXT("Avaialble Resolution: ") + Resolution.ToString());
	}
	
}

void UListDataObject_StringResolution::OnDataObjectInitialized()
{
	
	Super::OnDataObjectInitialized();
	
	if (DataDynamicGetter)
	{
		//We Got Value like this from the DynamicGetter: (X=2560,Y=1440)
		Debug::Print(TEXT("Resolution value  from the DynamicGetter: ") + DataDynamicGetter->GetValueAsString());
		//DataDynamicGetter->GetValueAsString()
	}
	
}
