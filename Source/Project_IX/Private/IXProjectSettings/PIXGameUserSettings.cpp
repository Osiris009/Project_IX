// Fill out your copyright notice in the Description page of Project Settings.


#include "IXProjectSettings/PIXGameUserSettings.h"

UPIXGameUserSettings* UPIXGameUserSettings::Get()
{
	if (GEngine)
	{
		return CastChecked<UPIXGameUserSettings>(GEngine->GetGameUserSettings());
	}
	return nullptr;
	
}
