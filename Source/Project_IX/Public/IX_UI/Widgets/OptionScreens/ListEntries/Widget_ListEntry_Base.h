// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"

#include "Widget_ListEntry_Base.generated.h"


class UCommonTextBlock;
class UListDataObject_Base; 

UCLASS(Abstract, BlueprintType, meta = (DisableNaiveTick))
class PROJECT_IX_API UWidget_ListEntry_Base : public UCommonUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
protected:
	//from IUserObjectListEntry
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	
	//The Child class should override this function to handle the initialization needed. Super call is expected  
	virtual void OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject);
	
private:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = "true"))
	UCommonTextBlock* CommonText_SettingDisplayName;

};
