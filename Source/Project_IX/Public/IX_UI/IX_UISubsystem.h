// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "IX_UI/Extra/UIEnumTypes.h"
#include "IX_UISubsystem.generated.h"


class UWidget_ConsumableSelector;
class UWidget_PrimaryLayout;
struct FGameplayTag;
class UWidget_ActivatableBase;
class UPIX_CommonButtonBase;

enum class EAsyncPushWidgetState : uint8
{
	OnCreatedBeforePush,
	AfterPush
};
// Fired once the primary layout is ready
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnButtonDescriptionTextUpdatedDelegate, UPIX_CommonButtonBase*, BroadcastingButton, FText, DescriptionText);


UCLASS()
class PROJECT_IX_API UIX_UISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	static UIX_UISubsystem* Get(const UObject* WorldContextObject);

	//~ Begin USubsyem Interface
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	//~ End USubsyem Interface

	UFUNCTION(BlueprintCallable)
	void RegisterCreatedPrimaryLayoutWidget(UWidget_PrimaryLayout* InCreatedWidget);

	void PushSoftWidgetToStackAsync(const FGameplayTag& InWidgetStackTag,
		TSoftClassPtr<UWidget_ActivatableBase> InSoftWidgetClass, 
		TFunction<void(EAsyncPushWidgetState,UWidget_ActivatableBase*)> AysncPushStateCallback);

	void PushConfirmScreenToModalStackAsync(EConfirmScreenType InScreenType, const FText& InScreenTitle,
		const FText& InScreenMsg, TFunction<void(EConfirmScreenButtonType)> ButtonClickedCallback);

	UPROPERTY(BlueprintAssignable)
	FOnButtonDescriptionTextUpdatedDelegate OnButtonDescriptionTextUpdated;
	
	UFUNCTION(BlueprintCallable, Category = "ConsumableUI")
	void PushConsumableWidgetToStack();
	
	UPROPERTY(Transient,BlueprintReadOnly)
	UWidget_ConsumableSelector* CreatedConsumableSelector = nullptr;
	
	UFUNCTION(BlueprintPure, Category = "ConsumableUI")
	UWidget_ConsumableSelector* GetConsumableSelector() const {
		return CreatedConsumableSelector;
	}
	
private:
	UPROPERTY(Transient)
	UWidget_PrimaryLayout* CreatedPrimaryLayout;
};
