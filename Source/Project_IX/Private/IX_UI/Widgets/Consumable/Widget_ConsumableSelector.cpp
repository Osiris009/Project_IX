// Fill out your copyright notice in the Description page of Project Settings.


#include "IX_UI/Widgets/Consumable/Widget_ConsumableSelector.h"

#include "IX_UI/Widgets/Consumable/Widget_ConsumableSlot.h"

#include "Components/VerticalBox.h"

// ── UUserWidget overrides ─────────────────────────────────────────────────────

void UWidget_ConsumableSelector::NativeConstruct()
{
    Super::NativeConstruct();
    // Slots built lazily on InitializeItems; nothing to do here.
}

void UWidget_ConsumableSelector::NativeDestruct()
{
    Super::NativeDestruct();
    SlotWidgets.Empty();
}

void UWidget_ConsumableSelector::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if (!bIsSelectorOpen || RuntimeItems.IsEmpty()) return;

    // ── Interpolate ScrollIndex toward target ─────────────────────────────
    //
    // Problem: naive FInterpTo breaks at the wrap boundary.
    // e.g. items=3, ScrollIndex=0.1 targeting 2.9 → would lerp the long way.
    //
    // Fix: compute the shortest-path delta, apply it, then re-wrap.
    //
    const float N = static_cast<float>(RuntimeItems.Num());
    float Delta = TargetScrollIndex - ScrollIndex;

    // Wrap delta into [-N/2, N/2]
    Delta = Delta - N * FMath::RoundToFloat(Delta / N);

    const bool bAlreadySnapped = FMath::Abs(Delta) < SnapThreshold;

    if (!bAlreadySnapped)
    {
        // FInterpTo-style: moves faster when far, slows as it approaches
        const float Step = Delta * FMath::Min(ScrollSpeed * InDeltaTime, 1.f);
        ScrollIndex = WrapFloat(ScrollIndex + Step);
    }
    else
    {
        ScrollIndex = WrapFloat(TargetScrollIndex);
    }

    // ── Push transforms to all slot widgets ──────────────────────────────
    UpdateAllDrumTransforms();

    // ── Change detection: fire delegate when we land on a new integer ─────
    const int32 Snapped = GetSnappedIndex();
    if (bAlreadySnapped && Snapped != LastSnappedIndex)
    {
        LastSnappedIndex = Snapped;
        if (RuntimeItems.IsValidIndex(Snapped))
        {
            OnSelectionChanged(RuntimeItems[Snapped]);
            OnConsumableSelected.Broadcast(RuntimeItems[Snapped]);
        }
    }
}

// ── Public API ────────────────────────────────────────────────────────────────

void UWidget_ConsumableSelector::InitializeItems(const TArray<FConsumableRuntimeData>& InItems)
{
    RuntimeItems = InItems;
    ScrollIndex       = 0.f;
    TargetScrollIndex = 0.f;
    LastSnappedIndex  = 0;

    if (!RuntimeItems.IsEmpty())
        ConfirmedItem = RuntimeItems[0];

    BuildSlots();
    OnItemsInitialized();
}

void UWidget_ConsumableSelector::BuildSlots()
{
    // ── Guard ─────────────────────────────────────────────────────────────
    if (!SlotContainer || !SlotWidgetClass) return;

    // ── Clear previous slots ──────────────────────────────────────────────
    SlotContainer->ClearChildren();
    SlotWidgets.Empty();

    const int32 N = RuntimeItems.Num();
    SlotWidgets.Reserve(N);

    for (int32 i = 0; i < N; ++i)
    {
        // NEW
        UWidget_ConsumableSlot* NewSlot = CreateWidget<UWidget_ConsumableSlot>(
            GetOwningPlayer(), SlotWidgetClass);

        if (!NewSlot) continue;
        NewSlot->InitSlot(RuntimeItems[i], i, N);

        // Use overlay-style layout — slots are positioned by RenderTransform,
        // not by VerticalBox layout. We add them all to the same container
        // so they share the same UMG parent, but their layout slot size
        // should be (0,0) with Fill=false so they don't push each other.
        //
        // In Blueprint: set the VerticalBox slot padding to 0, size to Auto.
        // All visual positioning is done in UpdateDrumTransform via SetRenderTransform.
        SlotContainer->AddChildToVerticalBox(NewSlot);
        SlotWidgets.Add(NewSlot);
    }

    // Apply initial transforms at rest
    UpdateAllDrumTransforms();
}

void UWidget_ConsumableSelector::UpdateAllDrumTransforms()
{
    for (UWidget_ConsumableSlot* ConsumableSlot  : SlotWidgets)
    {
        if (!ConsumableSlot) continue;
        ConsumableSlot->UpdateDrumTransform(
            ScrollIndex,
            SlotHeight,
            CenterScale,
            EdgeScale,
            EdgeOpacity,
            VisibilityRange
        );
    }
}

void UWidget_ConsumableSelector::UpdateItemCounts(const TMap<FName, int32>& InCountMap)
{
    // No rebuild — just update counts on existing slots
    for (int32 i = 0; i < RuntimeItems.Num(); ++i)
    {
        const int32* NewCount = InCountMap.Find(RuntimeItems[i].ItemID);
        if (NewCount)
        {
            RuntimeItems[i].Count = *NewCount;
            if (SlotWidgets.IsValidIndex(i) && SlotWidgets[i])
                SlotWidgets[i]->SetCount(*NewCount);
        }
    }

    // If the confirmed item ran out, snap to nearest valid
    if (!ConfirmedItem.ItemID.IsNone())
    {
        const int32* ConfirmedCount = InCountMap.Find(ConfirmedItem.ItemID);
        if (!ConfirmedCount || *ConfirmedCount <= 0)
        {
            const int32 Valid = FindNearestValidIndex(ScrollIndex, 1);
            TargetScrollIndex = static_cast<float>(Valid);
            // ConfirmedItem will update on next ConfirmSelection call
        }
    }
}

void UWidget_ConsumableSelector::UpdateCountsByType(
    int32 FruitCount, int32 HerbCount, int32 ThrowableCount)
{
    TMap<FName, int32> CountMap;
    for (const FConsumableRuntimeData& Item : RuntimeItems)
    {
        switch (Item.Type)
        {
        case EConsumableType::Fruit:     CountMap.Add(Item.ItemID, FruitCount);     break;
        case EConsumableType::Herb:      CountMap.Add(Item.ItemID, HerbCount);      break;
        case EConsumableType::Throwable: CountMap.Add(Item.ItemID, ThrowableCount); break;
        }
    }
    UpdateItemCounts(CountMap);
}

void UWidget_ConsumableSelector::ScrollNext()
{
    if (RuntimeItems.Num() < 2) return;

    // Find next valid item in the +1 direction
    const int32 Current = GetSnappedIndex();
    const int32 Next    = FindNearestValidIndex(static_cast<float>(Current), 1);

    // Compute shortest-path signed step (handles wrap correctly)
    float Delta = static_cast<float>(Next) - TargetScrollIndex;
    const float N = static_cast<float>(RuntimeItems.Num());
    Delta = Delta - N * FMath::RoundToFloat(Delta / N);

    TargetScrollIndex = WrapFloat(TargetScrollIndex + Delta);
}

void UWidget_ConsumableSelector::ScrollPrev()
{
    if (RuntimeItems.Num() < 2) return;

    const int32 Current = GetSnappedIndex();
    const int32 Prev    = FindNearestValidIndex(static_cast<float>(Current), -1);

    float Delta = static_cast<float>(Prev) - TargetScrollIndex;
    const float N = static_cast<float>(RuntimeItems.Num());
    Delta = Delta - N * FMath::RoundToFloat(Delta / N);

    TargetScrollIndex = WrapFloat(TargetScrollIndex + Delta);
}

void UWidget_ConsumableSelector::OpenSelector()
{
    if (bIsSelectorOpen) return;
    bIsSelectorOpen = true;

    // Snap to nearest valid item
    const int32 Valid = FindNearestValidIndex(ScrollIndex, 1);
    ScrollIndex       = static_cast<float>(Valid);
    TargetScrollIndex = ScrollIndex;
    LastSnappedIndex  = Valid;

    SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    UpdateAllDrumTransforms();
    OnSelectorOpened();
}

void UWidget_ConsumableSelector::CloseSelector()
{
    if (!bIsSelectorOpen) return;
    bIsSelectorOpen = false;
    SetVisibility(ESlateVisibility::Collapsed);
    OnSelectorClosed();
}

void UWidget_ConsumableSelector::ConfirmSelection()
{
    if (RuntimeItems.IsEmpty()) return;
    const int32 Idx = GetSnappedIndex();
    if (RuntimeItems.IsValidIndex(Idx))
    {
        ConfirmedItem = RuntimeItems[Idx];
        OnConsumableConfirmed.Broadcast(ConfirmedItem);
    }
}

void UWidget_ConsumableSelector::UseConfirmedItem()
{
    if (ConfirmedItem.ItemID.IsNone()) return;
    OnConsumableUsed.Broadcast(ConfirmedItem);
    OnItemUsed(ConfirmedItem);
}

int32 UWidget_ConsumableSelector::GetCurrentItemCount() const
{
    const int32 Idx = GetSnappedIndex();
    return RuntimeItems.IsValidIndex(Idx) ? RuntimeItems[Idx].Count : 0;
}

// ── Private helpers ───────────────────────────────────────────────────────────

float UWidget_ConsumableSelector::WrapFloat(float Index) const
{
    if (RuntimeItems.IsEmpty()) return 0.f;
    const float N = static_cast<float>(RuntimeItems.Num());
    return Index - N * FMath::FloorToFloat(Index / N);
}

int32 UWidget_ConsumableSelector::WrapInt(int32 Index) const
{
    if (RuntimeItems.IsEmpty()) return 0;
    const int32 N = RuntimeItems.Num();
    return ((Index % N) + N) % N;
}

int32 UWidget_ConsumableSelector::GetSnappedIndex() const
{
    return WrapInt(FMath::RoundToInt(ScrollIndex));
}

bool UWidget_ConsumableSelector::HasStock(int32 Index) const
{
    return RuntimeItems.IsValidIndex(Index) && RuntimeItems[Index].Count > 0;
}

int32 UWidget_ConsumableSelector::FindNearestValidIndex(float FromFloat, int32 Direction) const
{
    if (RuntimeItems.IsEmpty()) return 0;
    const int32 N    = RuntimeItems.Num();
    const int32 From = WrapInt(FMath::RoundToInt(FromFloat));

    // Direction should be +1 or -1
    // Search up to N-1 steps in that direction
    for (int32 i = 1; i < N; ++i)
    {
        const int32 Candidate = WrapInt(From + Direction * i);
        if (HasStock(Candidate)) return Candidate;
    }

    // Fallback: return current even if empty
    return From;
}