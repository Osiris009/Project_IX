// Fill out your copyright notice in the Description page of Project Settings.


#include "IX_UI/Widgets/Widget_ActivatableBase.h"
#include "BaseClasses/BasePlayerController.h"

ABasePlayerController* UWidget_ActivatableBase::GetOwningIXPlayerController()
{
	if (!CachedOwningPlayerController.IsValid())
	{
	 	CachedOwningPlayerController = GetOwningPlayer<ABasePlayerController>();
	}

    return CachedOwningPlayerController.IsValid() ? CachedOwningPlayerController.Get() : nullptr;
}
