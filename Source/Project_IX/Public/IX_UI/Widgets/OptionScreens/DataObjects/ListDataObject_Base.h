// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ListDataObject_Base.generated.h"


#define LIST_DATA_ACCESSOR(DataType,PropertyName)\
	FORCEINLINE DataType Get##PropertyName() const {return PropertyName;}\
	void Set##PropertyName(DataType In##PropertyName){PropertyName = In##PropertyName;}


UCLASS()
class PROJECT_IX_API UListDataObject_Base : public UObject
{
	GENERATED_BODY()
	
public:
	
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
	
protected:
	
	//
	virtual void OnDataObjectInitialized();
	
private:
	
	FName DataID;
	FText DataDisplayName;
	FText DescriptionRichText;
	FText DisabledReachText;
	TSoftObjectPtr<UTexture2D> SoftDescriptionImage;
	
	UPROPERTY(Transient)
	UListDataObject_Base* ParentData;
	
	
	
};
