// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonListView.h"
#include "PIX_CommonListView.generated.h"

class  UDataAsset_DataListEntryMapping;


UCLASS()
class PROJECT_IX_API UPIX_CommonListView : public UCommonListView
{
	GENERATED_BODY()
	
protected:
	//~~ UCommonListView interface
	virtual UUserWidget& OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable) override;
	
	virtual bool OnIsSelectableOrNavigableInternal(UObject* FirstSelectedItem) override;
	
	//~~ End of UCommonListView interface

private:
	
	//~~ UWidget interface
#if WITH_EDITOR	
	virtual void ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const override;
#endif
	//~~ End of UWidget interface
	
	UPROPERTY(EditAnywhere, Category = "PIX List Viwe Settiong")
	UDataAsset_DataListEntryMapping* DataListEntryMapping;
	
};
