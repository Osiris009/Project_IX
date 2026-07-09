// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "IX_UI/Extra/UIEnumTypes.h"
#include "ListDataObject_Base.generated.h"


#define LIST_DATA_ACCESSOR(DataType,PropertyName)\
	FORCEINLINE DataType Get##PropertyName() const {return PropertyName;}\
	void Set##PropertyName(DataType In##PropertyName){PropertyName = In##PropertyName;}


UCLASS(Abstract)
class PROJECT_IX_API UListDataObject_Base : public UObject
{
	GENERATED_BODY()
	
public:
	
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnListDataModifiedDelegate, UListDataObject_Base*,EOptionListDataModifyReason)
	FOnListDataModifiedDelegate OnListDataModified;


	LIST_DATA_ACCESSOR(FName,DataID)
	LIST_DATA_ACCESSOR(FText,DataDisplayName)
	LIST_DATA_ACCESSOR(FText,DescriptionRichText)
	LIST_DATA_ACCESSOR(FText,DisabledReachText)
	LIST_DATA_ACCESSOR(TSoftObjectPtr<UTexture2D>,SoftDescriptionImage)
	LIST_DATA_ACCESSOR(UListDataObject_Base*,ParentData)
	
	void InitDataObject();
	
	//Empty in the base class. Child class ListDataObject_Collection should override it. The function should return all the child data a tab has
	virtual TArray<UListDataObject_Base*> GetAllChildListData() const { return TArray<UListDataObject_Base*>();}
	virtual bool HasAnyChildListData() const { return false;}

	void SetShouldApplyChangeImimediately(bool bInShouldApplyRightAway){
		bShouldApplyChangeImimediately = bInShouldApplyRightAway;
	}
	

	//The child class should override this function to return true if the data object has a default value.
	//Child Going to Override this Functions to Provide Implemantation for reseting the value back to default value
	virtual bool HasDefaultValue() const { return false; }
	virtual bool CanResetBackToDefaultValue() const { return false; }
	virtual bool TryResetBackToDefaultValue() { return false; }

protected:
	
	//
	virtual void OnDataObjectInitialized();
	
	virtual void NotifyListDataModified(UListDataObject_Base* ModifiedData, 
		EOptionListDataModifyReason ModifyReason = EOptionListDataModifyReason::DirectlyModified);

private:
	
	FName DataID;
	FText DataDisplayName;
	FText DescriptionRichText;
	FText DisabledReachText;
	TSoftObjectPtr<UTexture2D> SoftDescriptionImage;
	
	UPROPERTY(Transient)
	UListDataObject_Base* ParentData;
	
	bool bShouldApplyChangeImimediately = false;
	
};
