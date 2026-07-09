// Fill out your copyright notice in the Description page of Project Settings.


#include "IX_UI/IX_UISubsystem.h"
#include "ProjectDebugHelper.h"
#include "IX_UI/Widgets/Widget_PrimaryLayout.h"
#include "Engine/AssetManager.h"
#include "IX_UI/Extra/UIGamePlayTags.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "IX_UI/Widgets/Widget_ActivatableBase.h"
#include "IX_UI/Widgets/Widget_ConfirmScreen.h"
#include "IXFuctionLibrary/UIFunctionLibrary.h"
#include  "IX_UI/Widgets/Consumable/Widget_ConsumableSelector.h"






UIX_UISubsystem* UIX_UISubsystem::Get(const UObject* WorldContextObject)
{
	if (GEngine)
	{
		UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);

		return UGameInstance::GetSubsystem<UIX_UISubsystem>(World->GetGameInstance());
	}

	return nullptr;
}

bool UIX_UISubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance())
	{
		TArray<UClass*> FoundClasses;
		GetDerivedClasses(GetClass(), FoundClasses);

		return FoundClasses.IsEmpty();
	}
	 
	return false;
}

void UIX_UISubsystem::RegisterCreatedPrimaryLayoutWidget(UWidget_PrimaryLayout* InCreatedWidget)
{
	check(InCreatedWidget);

	CreatedPrimaryLayout = InCreatedWidget;
	
	/*Debug::Print(TEXT("Primary layout widget stored"));*/
}

void UIX_UISubsystem::PushSoftWidgetToStackAsync(const FGameplayTag& InWidgetStackTag,
	TSoftClassPtr<UWidget_ActivatableBase> InSoftWidgetClass,
	TFunction<void(EAsyncPushWidgetState,
		UWidget_ActivatableBase*)> AysncPushStateCallback)

{
	check(!InSoftWidgetClass.IsNull());

	UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
		InSoftWidgetClass.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda(
			[InSoftWidgetClass, this, InWidgetStackTag, AysncPushStateCallback]()
			{
				UClass* LoadedWidgetClass = InSoftWidgetClass.Get();

				check(LoadedWidgetClass && CreatedPrimaryLayout);

				UCommonActivatableWidgetContainerBase* FoundWidgetStack = CreatedPrimaryLayout->FindWidgetStackByTag(InWidgetStackTag);

				UWidget_ActivatableBase* CreatedWidget = FoundWidgetStack->AddWidget<UWidget_ActivatableBase>(
					LoadedWidgetClass,
					[AysncPushStateCallback](UWidget_ActivatableBase& CreatedWidgetInstance)
					{
						AysncPushStateCallback(EAsyncPushWidgetState::OnCreatedBeforePush, &CreatedWidgetInstance);
					}
				);

				AysncPushStateCallback(EAsyncPushWidgetState::AfterPush, CreatedWidget);
			}
		)
	);
}

void UIX_UISubsystem::PushConfirmScreenToModalStackAsync(EConfirmScreenType InScreenType, const FText& InScreenTitle, const FText& InScreenMsg, TFunction<void(EConfirmScreenButtonType)> ButtonClickedCallback)
{
	UConfirmScreenInfoObject* CreatedInfoObject = nullptr;

	switch (InScreenType)
	{
	case EConfirmScreenType::Ok:

		CreatedInfoObject = UConfirmScreenInfoObject::CreateOkScreen(InScreenTitle, InScreenMsg);

		break;
	case EConfirmScreenType::YesNo:
		
		CreatedInfoObject = UConfirmScreenInfoObject::CreateYesNoScreen(InScreenTitle, InScreenMsg);

		break;
	case EConfirmScreenType::OKCancel:

		CreatedInfoObject = UConfirmScreenInfoObject::CreateOkCancelScreen(InScreenTitle, InScreenMsg); 

		break;


	case EConfirmScreenType::Unknown:
		break;
	default:
		break;
	}

	check(CreatedInfoObject);
	// For confirm screens, we assume the widget class is always the same, so we can directly load it here without needing to pass it in

	PushSoftWidgetToStackAsync(
		IXGameplayTags::IXUI_WidgetStack_Model,
		UUIFunctionLibrary::GetUISoftWidgetClassByTag(IXGameplayTags::IXUI_Widget_ConfirmScreen),
		[CreatedInfoObject, ButtonClickedCallback](EAsyncPushWidgetState InPushState, UWidget_ActivatableBase* PushedWidget)
		{
			if (InPushState == EAsyncPushWidgetState::OnCreatedBeforePush)
			{
				UWidget_ConfirmScreen* CreatedConfirmScreen = CastChecked<UWidget_ConfirmScreen>(PushedWidget);
				CreatedConfirmScreen-> InitConfirmScreen(CreatedInfoObject, ButtonClickedCallback);
			}
		}
	);	

}

void UIX_UISubsystem::PushConsumableWidgetToStack()
{
	PushSoftWidgetToStackAsync(
	IXGameplayTags::IXUI_WidgetStack_GameHUD,
	UUIFunctionLibrary::GetUISoftWidgetClassByTag(IXGameplayTags::IXUI_Widget_ConsumableToggle),
	[this](EAsyncPushWidgetState InPushState, UWidget_ActivatableBase* PushedWidget)
	{
		if (InPushState == EAsyncPushWidgetState::AfterPush)
		{

			UWidget_ActivatableBase* CreatedWidget = PushedWidget;
			CreatedConsumableSelector = CastChecked<UWidget_ConsumableSelector>(CreatedWidget);
			CreatedWidget->ActivateWidget(); // Example of configuring the widget after push
			
		}
	}
);

}