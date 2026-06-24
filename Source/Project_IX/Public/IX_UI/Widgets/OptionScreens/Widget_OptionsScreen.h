// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IX_UI/Widgets/Widget_ActivatableBase.h"
#include "Widget_OptionsScreen.generated.h"


class UOptionsDataRegistry;

class UProIXTabListWidgetBase;
class UPIX_CommonListView;


UCLASS(Abstract, BlueprintType, meta = (DisableNaiveTick))
class PROJECT_IX_API UWidget_OptionsScreen : public UWidget_ActivatableBase
{
	GENERATED_BODY()
	
protected:
	
	//~ Begin UWidget_ActivatableBase Interface
	virtual void NativeOnInitialized() override;	
	//~ End UWidget_ActivatableBase Interface

	//~ Begin UCommonActivatableWidget Interface
	virtual void NativeOnActivated() override;	

	virtual void NativeOnDeactivated() override;

	//~ End UCommonActivatableWidget Interface

private:
	//
	UOptionsDataRegistry* GetorCreateDataRegistry();

	UFUNCTION()
	void OnOptionsTabSelected(FName TabID);

	//BindedWidgets -------

	UPROPERTY(meta = (BindWidget))
	UProIXTabListWidgetBase* TabListWidget_OptionsTabs;
	
	UPROPERTY(meta = (BindWidget))
	UPIX_CommonListView* CommonListView_OptionsList;

	//BindedWidgets -------

/* 
	The data registry that this options screen will use to get all the data objects
	it needs to populate its options tabs. The registry is created by the options 
	screen's parent widget (the options menu) and passed down to the options screen after creation.
	Transient = don't save or load this data, as it will be re-created and re-initialized
	by the options screen's parent widget (the options menu) every time the options screen is created

*/
	UPROPERTY(Transient) 
	UOptionsDataRegistry* CreatedOwningDataRegistry;

	//Input Action Bindings -------
	UPROPERTY(EditDefaultsOnly, Category = "Project Options Screen", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle ResetAction;

	void OnResetBoundActionTriggered();
	void OnBackBoundActionTriggered();

	FUIActionBindingHandle ResetActionHandle;

};
