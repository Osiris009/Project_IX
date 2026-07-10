// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "GameplayTagContainer.h"
#include "IXDeveloperSettings.generated.h"

class UWidget_ActivatableBase;

UCLASS(Config = Game, defaultconfig, meta = (DisplayName = "Project IX Settings"))
class PROJECT_IX_API UIXDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public: 

	UPROPERTY(EditAnywhere, Config, Category = "WidgetRefrence", meta = (ForceInlineRow, Categories = "IXUI.Widget"))
	TMap< FGameplayTag, TSoftClassPtr<UWidget_ActivatableBase>> DefaultWidgetForWidgetStack;

	UPROPERTY(Config, EditAnywhere, Category = "Options Image Reference", meta = (ForceInlineRow, Categories = "IXUI.Image"))
	TMap< FGameplayTag, TSoftObjectPtr<UTexture2D> > OptionsScreenSoftImageMap;

};
