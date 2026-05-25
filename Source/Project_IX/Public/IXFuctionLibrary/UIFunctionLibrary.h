// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayTagContainer.h"
#include "UIFunctionLibrary.generated.h"


class UWidget_ActivatableBase;


UCLASS()
class PROJECT_IX_API UUIFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintPure, Category = "UI")
	static TSoftClassPtr<UWidget_ActivatableBase> GetUISoftWidgetClassByTag(UPARAM(meta = (Categories = "IXUI.Widget"))FGameplayTag InWidgetTag);
};
