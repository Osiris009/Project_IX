// Fill out your copyright notice in the Description page of Project Settings.


#include "IXFuctionLibrary/UIFunctionLibrary.h"
#include "IXProjectSettings/IXDeveloperSettings.h"	
#include "IX_UI/Widgets/Widget_ActivatableBase.h"


TSoftClassPtr<UWidget_ActivatableBase> UUIFunctionLibrary::GetUISoftWidgetClassByTag(UPARAM(meta = (Categories = "IXUI.Widget"))FGameplayTag InWidgetTag)
{
	const UIXDeveloperSettings* UIDevSettings =  GetDefault<UIXDeveloperSettings>();

	checkf(UIDevSettings->DefaultWidgetForWidgetStack.Contains(InWidgetTag),TEXT("Can't find widget %s"), *InWidgetTag.ToString())

	return UIDevSettings->DefaultWidgetForWidgetStack.FindRef(InWidgetTag);

}
