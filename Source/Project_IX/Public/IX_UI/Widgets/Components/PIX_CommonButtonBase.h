// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "PIX_CommonButtonBase.generated.h"


class UCommonTextBlock;


UCLASS(Abstract, BlueprintType, meta = (DisableNaiveTick))
class PROJECT_IX_API UPIX_CommonButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()
	/* Bound Widget*/

public:

	UFUNCTION(BlueprintCallable, Category = "PIX UI Button|Setters")
	void SetButtonText(FText InButtonText);


private:
	/// UUserWidget interface
	virtual void NativePreConstruct() override;	

	virtual void NativeOnCurrentTextStyleChanged() override;

	virtual void NativeOnHovered() override; 

	virtual void NativeOnUnhovered() override;


	UPROPERTY(meta = (BindWidgetOptional))
	UCommonTextBlock*  CommonnTextBlock_ButtonText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PIX UI Button", meta = (AllowPrivateAccess = "true"))
	FText ButtonDisplayText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PIX UI Button", meta = (AllowPrivateAccess = "true"))
	bool bUserUpperCaseForButtonText =	false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PIX UI Button", meta = (AllowPrivateAccess = "true"))
	FText ButtonDiscriptionText;

};


