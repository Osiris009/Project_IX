// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_OptionsDetailsView.generated.h"

class UCommonTextBlock;
class UCommonLazyImage;
class UCommonRichTextBlock;	

UCLASS(Abstract, BlueprintType, meta = (DisableNaiveTick))
class PROJECT_IX_API UWidget_OptionsDetailsView : public UUserWidget
{
	GENERATED_BODY()
	
private:	

	//*** BoundWidgets ***//

	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* CommonTextBlock_Titel;

	UPROPERTY(meta = (BindWidget))
	UCommonLazyImage* CommonLazyImage_DiscriptionImage;

	UPROPERTY(meta = (BindWidget))
	UCommonRichTextBlock* CommonRichText_Description;

	UPROPERTY(meta = (BindWidget))
	UCommonRichTextBlock* CommonRichText_DynamicDetails;
	UPROPERTY(meta = (BindWidget))
	UCommonRichTextBlock* CommonRichText_DisabledResons;
	//*** BoundWidets ***//

};
