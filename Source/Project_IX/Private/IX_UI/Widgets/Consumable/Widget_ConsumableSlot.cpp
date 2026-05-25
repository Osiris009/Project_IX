// Fill out your copyright notice in the Description page of Project Settings.


#include "IX_UI/Widgets/Consumable/Widget_ConsumableSlot.h"

#include "Components/Image.h"
#include "Engine/Texture2D.h"

void UWidget_ConsumableSlot::InitSlot(const FConsumableRuntimeData& InData,
                                       int32 InSlotIndex, int32 InTotalItems)
{
    RuntimeData  = InData;
    SlotIndex    = InSlotIndex;
    TotalItems   = InTotalItems;

    if (SlotIcon)
    {
        UTexture2D* Tex = nullptr;
        if (!InData.Icon.IsNull())
        {
            Tex = InData.Icon.Get();
            if (!Tex) Tex = InData.Icon.LoadSynchronous(); // safe: small UI textures
        }
        Tex ? SlotIcon->SetBrushFromTexture(Tex, false)
            : SlotIcon->SetBrushFromTexture(nullptr);
    }

    OnSlotInitialized(InData);
}

void UWidget_ConsumableSlot::UpdateDrumTransform(
    float ScrollIndex,
    float SlotHeight,
    float MaxScale,
    float MinScale,
    float MinOpacity,
    float VisibilityRange)
{
    if (TotalItems == 0) return;

    // ── Shortest-path wrap distance ───────────────────────────────────
    // Raw distance on the number line
    float Distance = static_cast<float>(SlotIndex) - ScrollIndex;

    // Wrap into [-N/2, N/2] so the drum loops correctly
    const float N = static_cast<float>(TotalItems);
    Distance = Distance - N * FMath::RoundToFloat(Distance / N);

    // Store normalized version for Blueprint
    NormalizedDistance = FMath::Clamp(Distance / FMath::Max(VisibilityRange, 1.f), -1.f, 1.f);

    // ── Visibility cull — hide slots beyond render range ──────────────
    const float AbsDist = FMath::Abs(Distance);
    if (AbsDist > VisibilityRange + 0.5f)
    {
        SetVisibility(ESlateVisibility::Collapsed);
        return;
    }
    SetVisibility(ESlateVisibility::SelfHitTestInvisible);

    // ── SmoothStep for natural ease at edges ──────────────────────────
    // t = 0 at center, 1 at VisibilityRange
    const float t = FMath::SmoothStep(0.f, VisibilityRange, AbsDist);

    // ── Apply transforms (C++ owns these, Blueprint never touches them) ─
    FWidgetTransform T;
    T.Translation = FVector2D(0.f, Distance * SlotHeight);
    const float S = FMath::Lerp(MaxScale, MinScale, t);
    T.Scale       = FVector2D(S, S);
    SetRenderTransform(T);
    SetRenderOpacity(FMath::Lerp(1.f, MinOpacity, t));

    // Fire BP event for extra effects (tint, glow, etc.)
    OnDrumTransformUpdated(NormalizedDistance);
}

void UWidget_ConsumableSlot::SetCount(int32 NewCount)
{
    RuntimeData.Count = NewCount;
    OnCountChanged(NewCount);
}