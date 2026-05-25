// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "IX_UI/Extra/ConsumableTypes.h"
#include "ConsumableItemData.generated.h"


UCLASS(BlueprintType)
class PROJECT_IX_API UConsumableItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Consumable")
	FName ItemID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Consumable")
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Consumable")
	TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Consumable")
	EConsumableType Type = EConsumableType::Fruit;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Consumable|Gameplay")
	float HealAmount = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Consumable|Gameplay")
	float CooldownSeconds = 0.f;

	// Converts this asset + a live count into a runtime struct
	FConsumableRuntimeData MakeRuntimeData(int32 Count) const
	{
		FConsumableRuntimeData R;
		R.ItemID          = ItemID;
		R.DisplayName     = DisplayName;
		R.Icon            = Icon;
		R.Type            = Type;
		R.Count           = Count;
		R.HealAmount      = HealAmount;
		R.CooldownSeconds = CooldownSeconds;
		return R;
	}

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("ConsumableItem", ItemID);
	}
};
