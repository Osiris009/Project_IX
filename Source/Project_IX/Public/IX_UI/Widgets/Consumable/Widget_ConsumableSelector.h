// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IX_UI/Widgets/Widget_ActivatableBase.h"
#include "IX_UI/Extra/ConsumableTypes.h"
#include "Widget_ConsumableSelector.generated.h"

/**
 * 
 */
class UWidget_ConsumableSlot;
class UVerticalBox;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConsumableSelected,  const FConsumableRuntimeData&, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConsumableConfirmed, const FConsumableRuntimeData&, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConsumableUsed,      const FConsumableRuntimeData&, Item);

UCLASS(Abstract, BlueprintType, Blueprintable)
class PROJECT_IX_API UWidget_ConsumableSelector : public UWidget_ActivatableBase
{
    GENERATED_BODY()

public:
    // ── Public API ────────────────────────────────────────────────────

    /** PlayerController calls this once on HUD init or inventory change. */
    UFUNCTION(BlueprintCallable, Category = "Consumable|Selector")
    void InitializeItems(const TArray<FConsumableRuntimeData>& InItems);

    /**
     * Lightweight count refresh. Does NOT rebuild slots.
     * Call this every time inventory changes.
     */
    UFUNCTION(BlueprintCallable, Category = "Consumable|Selector")
    void UpdateItemCounts(const TMap<FName, int32>& InCountMap);

    /** Convenience wrapper — maps type to all matching item IDs. */
    UFUNCTION(BlueprintCallable, Category = "Consumable|Selector")
    void UpdateCountsByType(int32 FruitCount, int32 HerbCount, int32 ThrowableCount);

    /** Input: increment TargetScrollIndex by 1 (wraps). */
    UFUNCTION(BlueprintCallable, Category = "Consumable|Selector")
    void ScrollNext();

    /** Input: decrement TargetScrollIndex by 1 (wraps). */
    UFUNCTION(BlueprintCallable, Category = "Consumable|Selector")
    void ScrollPrev();

    UFUNCTION(BlueprintCallable, Category = "Consumable|Selector")
    void OpenSelector();

    UFUNCTION(BlueprintCallable, Category = "Consumable|Selector")
    void CloseSelector();

    /** Locks in whatever ScrollIndex is currently resting on. */
    UFUNCTION(BlueprintCallable, Category = "Consumable|Selector")
    void ConfirmSelection();

    /** Broadcasts OnConsumableUsed with ConfirmedItem. */
    UFUNCTION(BlueprintCallable, Category = "Consumable|Selector")
    void UseConfirmedItem();

    UFUNCTION(BlueprintPure, Category = "Consumable|Selector")
    FConsumableRuntimeData GetConfirmedItem() const { return ConfirmedItem; }

    UFUNCTION(BlueprintPure, Category = "Consumable|Selector")
    bool IsSelectorOpen() const { return bIsSelectorOpen; }

    UFUNCTION(BlueprintPure, Category = "Consumable|Selector")
    int32 GetCurrentItemCount() const;

    // ── Delegates ────────────────────────────────────────────────────

    UPROPERTY(BlueprintAssignable, Category = "Consumable|Selector")
    FOnConsumableSelected OnConsumableSelected;

    UPROPERTY(BlueprintAssignable, Category = "Consumable|Selector")
    FOnConsumableConfirmed OnConsumableConfirmed;

    UPROPERTY(BlueprintAssignable, Category = "Consumable|Selector")
    FOnConsumableUsed OnConsumableUsed;

protected:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct()  override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    // ── BindWidget ────────────────────────────────────────────────────

    /** The container that holds all slot widgets. Must be named "SlotContainer" in BP. */
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Consumable|Selector")
    TObjectPtr<UVerticalBox> SlotContainer;

    /** Blueprint class to spawn for each slot. Set in BP defaults. */
    UPROPERTY(EditDefaultsOnly, Category = "Consumable|Config")
    TSubclassOf<UWidget_ConsumableSlot> SlotWidgetClass;

    // ── Drum scroll config (tunable in BP defaults) ───────────────────

    UPROPERTY(EditDefaultsOnly, Category = "Consumable|DrumScroll")
    float SlotHeight = 75.f;

    /** How fast ScrollIndex chases TargetScrollIndex. Higher = snappier. */
    UPROPERTY(EditDefaultsOnly, Category = "Consumable|DrumScroll")
    float ScrollSpeed = 14.f;

    /** Scale at dead center (distance = 0). */
    UPROPERTY(EditDefaultsOnly, Category = "Consumable|DrumScroll")
    float CenterScale = 1.2f;

    /** Scale at VisibilityRange distance. */
    UPROPERTY(EditDefaultsOnly, Category = "Consumable|DrumScroll")
    float EdgeScale = 0.7f;

    /** Minimum opacity at VisibilityRange distance. */
    UPROPERTY(EditDefaultsOnly, Category = "Consumable|DrumScroll")
    float EdgeOpacity = 0.3f;

    /**
     * How many slots away from center are still rendered.
     * 1.5 = only prev/curr/next visible. 2.5 = five slots visible. Etc.
     */
    UPROPERTY(EditDefaultsOnly, Category = "Consumable|DrumScroll")
    float VisibilityRange = 1.5f;

    /**
     * Snap threshold: if |ScrollIndex - TargetScrollIndex| < this,
     * snap exactly and stop ticking transform updates.
     */
    UPROPERTY(EditDefaultsOnly, Category = "Consumable|DrumScroll")
    float SnapThreshold = 0.01f;

    // ── Blueprint hooks ───────────────────────────────────────────────

    UFUNCTION(BlueprintImplementableEvent, Category = "Consumable|Selector")
    void OnItemsInitialized();

    UFUNCTION(BlueprintImplementableEvent, Category = "Consumable|Selector")
    void OnSelectorOpened();

    UFUNCTION(BlueprintImplementableEvent, Category = "Consumable|Selector")
    void OnSelectorClosed();

    UFUNCTION(BlueprintImplementableEvent, Category = "Consumable|Selector")
    void OnSelectionChanged(const FConsumableRuntimeData& NewItem);

    UFUNCTION(BlueprintImplementableEvent, Category = "Consumable|Selector")
    void OnItemUsed(const FConsumableRuntimeData& Item);

private:
    // ── Runtime state ─────────────────────────────────────────────────

    UPROPERTY(VisibleAnywhere, Category = "Consumable|State")
    TArray<FConsumableRuntimeData> RuntimeItems;

    /** Spawned slot widgets, index-aligned with RuntimeItems. */
    UPROPERTY()
    TArray<TObjectPtr<UWidget_ConsumableSlot>> SlotWidgets;

    /**
     * Fractional index: 0.0 = resting on item 0, 1.7 = 70% toward item 2.
     * This is the ONLY source of truth for what's selected.
     */
    float ScrollIndex = 0.f;

    /** ScrollIndex interpolates toward this. Set by ScrollNext/ScrollPrev. */
    float TargetScrollIndex = 0.f;

    UPROPERTY(VisibleAnywhere, Category = "Consumable|State")
    FConsumableRuntimeData ConfirmedItem;

    bool bIsSelectorOpen = false;

    /** Last integer index ScrollIndex was snapped to. Used for change detection. */
    int32 LastSnappedIndex = 0;

    // ── Helpers ───────────────────────────────────────────────────────

    /**
     * Wraps any float index into [0, N).
     * Works for negative values: WrapFloat(-0.3, 3) = 2.7
     */
    float WrapFloat(float Index) const;

    /** Integer wrap for array access. */
    int32 WrapInt(int32 Index) const;

    /** Builds slot widgets from scratch. Called once in InitializeItems. */
    void BuildSlots();

    /** Push current transform state to all slot widgets this frame. */
    void UpdateAllDrumTransforms();

    /** Find the nearest valid (count > 0) index from a given float position. */
    int32 FindNearestValidIndex(float FromFloat, int32 Direction) const;

    /** Returns true if item at Index has count > 0. */
    bool HasStock(int32 Index) const;

    /** Compute the snapped integer from current ScrollIndex. */
    int32 GetSnappedIndex() const;
};