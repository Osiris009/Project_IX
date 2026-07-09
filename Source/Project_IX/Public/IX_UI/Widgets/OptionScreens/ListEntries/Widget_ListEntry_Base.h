// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "IX_UI/Extra/UIEnumTypes.h"

#include "Widget_ListEntry_Base.generated.h"


class UCommonTextBlock;
class UListDataObject_Base; 

UCLASS(Abstract, BlueprintType, meta = (DisableNaiveTick))
class PROJECT_IX_API UWidget_ListEntry_Base : public UCommonUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:	

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On List Entry Widget Hovered"))
	void BP_OnOwningListDataObjectSet(bool bWasHovered, bool bIsEntryWidgetStillSelected);


	void NativeOnListEntryWidgetHovered(bool bWasHovered);

protected:

	//The child widget blueprint should override this function for the gamepad interaction to function properly
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Get Widget To Focus For Gamepad"))
	UWidget* BP_GetWidgetToFocusForGamepad() const;

	//from IUserObjectListEntry
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	
	//The Child class should override this function to handle the initialization needed. Super call is expected  
	virtual void OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject);
	
	//This child class should override this function to update the ui values after the data object has been modified. Super call not needed 
	virtual void OnOwningListDataObjectModified(UListDataObject_Base* OwningModifiedData, EOptionListDataModifyReason ModifyReason);

	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;

	void SelectThisListEntryWidget();

private:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = "true"))
	UCommonTextBlock* CommonText_SettingDisplayName;

};
