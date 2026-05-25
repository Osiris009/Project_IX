// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "IX_UI/Extra/ConsumableTypes.h"

#include "Widget_ConsumableSlot.generated.h"

/**
 * 
 */
class UImage;
class UTextBlock;

/**
 * One card in the drum strip.
 *
 * Key design: the slot owns its SlotIndex and data permanently after Init().
 * The SELECTOR never reassigns data mid-scroll.
 * The selector only sets ScrollIndex each tick; the slot computes its own transform.
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class PROJECT_IX_API UWidget_ConsumableSlot : public UCommonUserWidget
{
    GENERATED_BODY()

public:
    /**
     * Called once during selector initialization.
     * Loads icon (sync — small textures). Never called again during scroll.
     */
    UFUNCTION(BlueprintCallable, Category = "Consumable|Slot")
    void InitSlot(const FConsumableRuntimeData& InData, int32 InSlotIndex, int32 InTotalItems);

    /**
     * Called every tick by the selector.
     * ScrollIndex is a float: e.g. 1.7 means "70% scrolled toward item 1".
     * The slot computes its own distance and applies transform.
     */
    void UpdateDrumTransform(float ScrollIndex, float SlotHeight,
                              float MaxScale, float MinScale,
                              float MinOpacity, float VisibilityRange);

    /** Live count update — no rebuild, just refreshes text/visibility. */
    UFUNCTION(BlueprintCallable, Category = "Consumable|Slot")
    void SetCount(int32 NewCount);

    UFUNCTION(BlueprintPure, Category = "Consumable|Slot")
    FConsumableRuntimeData GetRuntimeData() const { return RuntimeData; }

    UFUNCTION(BlueprintPure, Category = "Consumable|Slot")
    int32 GetSlotIndex() const { return SlotIndex; }

    /** True if this slot has items available. */
    UFUNCTION(BlueprintPure, Category = "Consumable|Slot")
    bool HasStock() const { return RuntimeData.Count > 0; }

protected:
    // ── Bind in Blueprint ─────────────────────────────────────────────
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Consumable|Slot")
    TObjectPtr<UImage> SlotIcon;

    // ── State (read-only from Blueprint for anim, etc.) ───────────────
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Consumable|Slot")
    FConsumableRuntimeData RuntimeData;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Consumable|Slot")
    int32 SlotIndex = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Consumable|Slot")
    int32 TotalItems = 0;

    /**
     * Normalized distance from center [-1..1] this frame.
     * Exposed so Blueprint can drive additional effects (e.g. color tint).
     */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Consumable|Slot")
    float NormalizedDistance = 0.f;

    // ── Blueprint hooks ───────────────────────────────────────────────

    /** Called once after InitSlot. Set up icon, name text, etc. */
    UFUNCTION(BlueprintImplementableEvent, Category = "Consumable|Slot")
    void OnSlotInitialized(const FConsumableRuntimeData& Data);

    /**
     * Called every tick. NormalizedDistance in [-1..1]:
     *   0   = dead center (current)
     *  ±1   = one full slot away
     * Use this to drive material params, color tints, etc.
     * Do NOT set translation/scale/opacity here — C++ owns those.
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Consumable|Slot")
    void OnDrumTransformUpdated(float InNormalizedDistance);

    /** Called when count changes so Blueprint can flash/animate. */
    UFUNCTION(BlueprintImplementableEvent, Category = "Consumable|Slot")
    void OnCountChanged(int32 NewCount);
};