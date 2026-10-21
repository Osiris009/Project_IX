// Fill out your copyright notice in the Description page of Project Settings.


#include "IXProjectSettings/PIXLoadingScreenSettings.h"
#include "Blueprint/UserWidget.h"

TSubclassOf<UUserWidget> UPIXLoadingScreenSettings::GetLoadingScreenWidgetClassChecked() const 
{
    checkf(!SoftLoadingScreenWidgetClass.IsNull(), TEXT("Forgot to assign a valid widget blueprint in the project settings as loading screen"));

    TSubclassOf<UUserWidget> LoadedLoadingScreenWidget = SoftLoadingScreenWidgetClass.LoadSynchronous();

    return LoadedLoadingScreenWidget;
}