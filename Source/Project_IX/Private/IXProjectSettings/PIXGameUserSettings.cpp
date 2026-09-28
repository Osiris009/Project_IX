// Fill out your copyright notice in the Description page of Project Settings.


#include "IXProjectSettings/PIXGameUserSettings.h"

UPIXGameUserSettings::UPIXGameUserSettings()
	: OverallVolume(1.f)
	,MusicVolume(1.f)
	,SFXVolume(1.f)
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

void UPIXGameUserSettings::SetMusicVolume(float InVolume)
{
	MusicVolume = InVolume;
}

void UPIXGameUserSettings::SetSFXVolume(float InVolume)
{
	SFXVolume = InVolume;
}

void UPIXGameUserSettings::SetAllowBackgroundAudio(bool bIsAllowed)
{
	bAllowBackgroundAudio = bIsAllowed;
}

void UPIXGameUserSettings::SetUseHDRAudioMode(bool bIsAllowed)
{
	bUseHDRAudioMode = bIsAllowed;
}

float UPIXGameUserSettings::GetCurrentDisplayGamma() const
{
	if (GEngine)
	{
		return GEngine->GetDisplayGamma();
	}
	return 0.0f;
}

void UPIXGameUserSettings::SetCurrentDisplayGamma(float InNewGamma)
{
	if (GEngine)
	{
		GEngine->DisplayGamma = InNewGamma;
	}
}
