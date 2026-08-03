// Fill out your copyright notice in the Description page of Project Settings.


#include "IXProjectSettings/PIXGameUserSettings.h"

UPIXGameUserSettings::UPIXGameUserSettings()
	: OverallVolume(1.f)
{
	
}

UPIXGameUserSettings* UPIXGameUserSettings::Get()
{
	if (GEngine)
	{
		return CastChecked<UPIXGameUserSettings>(GEngine->GetGameUserSettings());
	}
	return nullptr;
	
}

void UPIXGameUserSettings::SetOverallVolume(float InVolume)
{
	OverallVolume = InVolume;	
	//The Actual logic for controlling volume goes here 
}
