// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseClasses/BasePlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "IX_UI/Extra/ConsumableTypes.h"
#include "GameplayTagContainer.h"
#include "InputActionValue.h"
#include "IX_UI/Widgets/Consumable/Widget_ConsumableSelector.h"
#include "IX_UI/IX_UISubsystem.h"

void ABasePlayerController::BeginPlay()
{
	Super::BeginPlay();


	TArray<AActor*>FoundCamera;
	UGameplayStatics::GetAllActorsOfClassWithTag(this, ACameraActor::StaticClass(),FName("Default"),FoundCamera);

	if (!FoundCamera.IsEmpty())
	{
		SetViewTarget(FoundCamera[0]);
	}
}


void ABasePlayerController::EnsureConsumableSelectorCreated()
{
    if (ConsumableSelectorWidget || ConsumableSelectorClass.IsNull()) return;

    UClass* SelectorClass = ConsumableSelectorClass.LoadSynchronous();
    if (!SelectorClass) return;

    ConsumableSelectorWidget = CreateWidget<UWidget_ConsumableSelector>(this, SelectorClass);
    if (!ConsumableSelectorWidget) return;

    // Wire delegates
    ConsumableSelectorWidget->OnConsumableConfirmed.AddDynamic(
        this, &ABasePlayerController::OnConsumableConfirmed);
    ConsumableSelectorWidget->OnConsumableUsed.AddDynamic(
        this, &ABasePlayerController::OnConsumableUsed);

    // ── Build runtime items from data assets ──────────────────────────
    TArray<FConsumableRuntimeData> RuntimeItems;

    for (const FConsumableSlotConfig& Config : ConsumableSlotConfigs)
    {
        if (Config.ItemData.IsNull()) continue;

        // LoadSynchronous is fine here — called once at startup, small assets
        UConsumableItemData* Asset = Config.ItemData.LoadSynchronous();
        if (!Asset) continue;

        // MakeRuntimeData is the helper we put on the data asset
        FConsumableRuntimeData RuntimeData = Asset->MakeRuntimeData(Config.InitialCount);
        RuntimeItems.Add(RuntimeData);
    }

    if (RuntimeItems.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("[Consumable] No valid items in ConsumableSlotConfigs!"));
        return;
    }

    ConsumableSelectorWidget->InitializeItems(RuntimeItems);
}

UWidget_ConsumableSelector* ABasePlayerController::GetConsumableSelector() const
{
    return ConsumableSelectorWidget;
}

// ── Delegate receivers ────────────────────────────────────────────────────────

void ABasePlayerController::OnConsumableConfirmed(const FConsumableRuntimeData& Item)
{
    UE_LOG(LogTemp, Log, TEXT("[Consumable] Confirmed: %s"), *Item.ItemID.ToString());

    // Example:
    // if (ADurgaCharacter* Char = Cast<ADurgaCharacter>(GetPawn()))
    //     Char->SetActiveConsumable(Item);
}

void ABasePlayerController::OnConsumableUsed(const FConsumableRuntimeData& Item)
{
    UE_LOG(LogTemp, Log, TEXT("[Consumable] Used: %s  Type: %d"),
        *Item.ItemID.ToString(), (int32)Item.Type);

    switch (Item.Type)
    {
    case EConsumableType::Herb:
        // Cast<ADurgaCharacter>(GetPawn())->ApplyHeal(Item.HealAmount);
        break;
    case EConsumableType::Fruit:
        // Cast<ADurgaCharacter>(GetPawn())->ApplyBuff(FruitBuffTag);
        break;
    case EConsumableType::Throwable:
        // Cast<ADurgaCharacter>(GetPawn())->EnterThrowMode(Item);
        break;
    default:
        break;
    }
}

// ── Input handlers ────────────────────────────────────────────────────────────

void ABasePlayerController::Handle_Tab_Pressed()
{
    if (GetWorldTimerManager().IsTimerActive(TabHoldTimerHandle)) return;

    bTabHoldDetected = false;

    GetWorldTimerManager().SetTimer(
        TabHoldTimerHandle,
        [this]()
        {
            bTabHoldDetected = true;
            // Blueprint pushes + activates the widget via CommonUI stack
            OnHoldDetected();
        },
        0.2f,
        false);
}

void ABasePlayerController::Handle_Tab_Released()
{
    GetWorldTimerManager().ClearTimer(TabHoldTimerHandle);

    UIX_UISubsystem* UI = UIX_UISubsystem::Get(this);
    UWidget_ConsumableSelector* Sel = UI ? UI->GetConsumableSelector() : nullptr;

    if (bTabHoldDetected)
    {
        // Hold release — confirm whatever is selected, use it, close
        if (Sel && Sel->IsSelectorOpen())
        {
            Sel->ConfirmSelection();
            Sel->UseConfirmedItem(); // renamed from UseSelectedItem
        }
        OnHoldReleased(); // BP pops the widget off the CommonUI stack
    }
    else
    {
        // Quick tap — use last confirmed item, selector never opened
        if (Sel) Sel->UseConfirmedItem();
    }

    bTabHoldDetected = false;
}

void ABasePlayerController::Handle_ScrollNext()
{
    if (!ConsumableSelectorWidget || !ConsumableSelectorWidget->IsSelectorOpen()) return;
    ConsumableSelectorWidget->ScrollNext();
}

void ABasePlayerController::Handle_ScrollPrev()
{
    if (!ConsumableSelectorWidget || !ConsumableSelectorWidget->IsSelectorOpen()) return;
    ConsumableSelectorWidget->ScrollPrev();
}

