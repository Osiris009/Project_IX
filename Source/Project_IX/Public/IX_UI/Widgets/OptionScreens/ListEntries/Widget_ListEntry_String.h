// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IX_UI/Widgets/OptionScreens/ListEntries/Widget_ListEntry_Base.h"
#include "Widget_ListEntry_String.generated.h"

class UPIX_CommonButtonBase;
class UPIXCommonRotator;
class UListDataObject_String;

UCLASS(Abstract, BlueprintType, meta = (DisableNaiveTick))
class PROJECT_IX_API UWidget_ListEntry_String : public UWidget_ListEntry_Base
{
	GENERATED_BODY()
	
protected:
	
	//~ Begin UWidget interface
	virtual void NativeOnInitialized() override;

	virtual void OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject) override;
	

	virtual void OnOwningListDataObjectModified(UListDataObject_Base* OwningModifiedData, EOptionListDataModifyReason ModifyReason) override;

private:

	void OnPreviousOptionButtonClicked();
	void OnNextOptionButtonClicked();

	UPROPERTY(BlueprintReadOnly,meta = (BindWidget, AllowPrivateAccess = "true"))
	UPIX_CommonButtonBase* CommonButton_PreviousOption;
	
	UPROPERTY(BlueprintReadOnly,meta = (BindWidget, AllowPrivateAccess = "true")) 
	UPIXCommonRotator* CommonRotator_AvailableOptions;

	UPROPERTY(BlueprintReadOnly,meta = (BindWidget, AllowPrivateAccess = "true"))
	UPIX_CommonButtonBase* CommonButton_NextOption;
	
	UPROPERTY(Transient)
	UListDataObject_String* CachedOwningStringDataObject;
	
};

  