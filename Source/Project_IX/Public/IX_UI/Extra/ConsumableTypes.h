#pragma once
#include "CoreMinimal.h"
#include "ConsumableTypes.generated.h"

UENUM(BlueprintType)
enum class EConsumableType : uint8
{
    Fruit      UMETA(DisplayName = "Fruit"),
    Herb       UMETA(DisplayName = "Herb"),
    Throwable  UMETA(DisplayName = "Throwable"),
};

// Runtime pairing of item definition + live count
// Kept as a struct (not UObject) — zero GC pressure
USTRUCT(BlueprintType)
struct PROJECT_IX_API FConsumableRuntimeData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ItemID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftObjectPtr<UTexture2D> Icon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EConsumableType Type = EConsumableType::Fruit;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Count = 0;

    // Optional gameplay fields — unused by UI, read by ability system
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float HealAmount = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CooldownSeconds = 0.f;

    bool IsValid() const { return !ItemID.IsNone() && Count > 0; }
};