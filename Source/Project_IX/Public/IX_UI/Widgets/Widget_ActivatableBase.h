// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "Widget_ActivatableBase.generated.h"


class ABasePlayerController;

UCLASS(Abstract, BlueprintType, meta = (DisableNaiveTick))
class PROJECT_IX_API UWidget_ActivatableBase : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:

	UFUNCTION(BlueprintPure, Category = "Widget Activatable Base")
	ABasePlayerController* GetOwningIXPlayerController();

private:

	TWeakObjectPtr<ABasePlayerController> CachedOwningPlayerController;
	
};
