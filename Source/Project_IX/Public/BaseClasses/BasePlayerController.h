// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "IX_UI/Widgets/Consumable/ConsumableItemData.h"
#include "BasePlayerController.generated.h"



class UWidget_ConsumableSelector;

// Add this struct above the class — pairs an asset with its initial count
USTRUCT(BlueprintType)
struct FConsumableSlotConfig
{
	GENERATED_BODY()

	// Assign DA_Herb / DA_Fruit / DA_Throwable here in BP defaults
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UConsumableItemData> ItemData;

	// Starting count — override from inventory later via UpdateItemCounts
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 InitialCount = 0;
};


UCLASS()
class  ABasePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	// ── Selector access ───────────────────────────────────────────────────
	UFUNCTION(BlueprintPure, Category = "Consumable|Input")
	UWidget_ConsumableSelector* GetConsumableSelector() const;

	// ── Delegate receivers ────────────────────────────────────────────────
	// Updated to FConsumableRuntimeData — matches new selector delegates
	UFUNCTION()
	void OnConsumableConfirmed(const FConsumableRuntimeData& Item);

	UFUNCTION()
	void OnConsumableUsed(const FConsumableRuntimeData& Item);

	// ── Input handlers (called from Enhanced Input bindings or BP) ────────
	UFUNCTION(BlueprintCallable, Category = "Consumable|Input")
	void Handle_Tab_Pressed();

	UFUNCTION(BlueprintCallable, Category = "Consumable|Input")
	void Handle_Tab_Released();

	/**
	 * Call these from your scroll input binding (gamepad stick or mouse wheel).
	 * Each call = one discrete step. Do NOT accumulate — just call once per
	 * input event. The selector's FInterpTo handles the smooth animation.
	 */
	UFUNCTION(BlueprintCallable, Category = "Consumable|Input")
	void Handle_ScrollNext();

	UFUNCTION(BlueprintCallable, Category = "Consumable|Input")
	void Handle_ScrollPrev();

	

	// ── Blueprint events ──────────────────────────────────────────────────
	UFUNCTION(BlueprintImplementableEvent, Category = "Consumable|Input")
	void OnHoldDetected();

	UFUNCTION(BlueprintImplementableEvent, Category = "Consumable|Input")
	void OnHoldReleased();

	UPROPERTY(Transient)
	TObjectPtr<UWidget_ConsumableSelector> ConsumableSelectorWidget;
	
	// Set these in BP_DurgaPlayerController Class Defaults
	UPROPERTY(EditDefaultsOnly, Category = "Consumable|Config")
	TArray<FConsumableSlotConfig> ConsumableSlotConfigs;
	
protected:

	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly, Category = "Consumable|Config")
	TSoftClassPtr<UWidget_ConsumableSelector> ConsumableSelectorClass;

	void EnsureConsumableSelectorCreated();

private:
	FTimerHandle TabHoldTimerHandle;
	bool         bTabHoldDetected = false;
	
	
};
