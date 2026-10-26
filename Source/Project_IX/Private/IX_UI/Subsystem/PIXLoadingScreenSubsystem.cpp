// Fill out your copyright notice in the Description page of Project Settings.


#include "IX_UI/Subsystem/PIXLoadingScreenSubsystem.h"
#include "PreLoadScreenManager.h"
#include "IXProjectSettings/PIXLoadingScreenSettings.h"
#include "Blueprint/UserWidget.h"


bool UPIXLoadingScreenSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance())
	{
		//
		TArray<UClass*> FoundClasses;
		GetDerivedClasses(GetClass(), FoundClasses);

		return FoundClasses.IsEmpty();
	}

	return false;
}

void UPIXLoadingScreenSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	FCoreUObjectDelegates::PreLoadMapWithContext.AddUObject(this,&ThisClass::OnMapPreloaded);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this,&ThisClass::OnMapPostLoad);
}

void UPIXLoadingScreenSubsystem::Deinitialize()
{
	FCoreUObjectDelegates::PreLoadMapWithContext.RemoveAll(this);
	FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this);
}

UWorld* UPIXLoadingScreenSubsystem::GetTickableGameObjectWorld() const
{
	if (UGameInstance* OwningGameInstance = GetGameInstance())
	{
		return OwningGameInstance->GetWorld();
	}
	return nullptr;
}

void UPIXLoadingScreenSubsystem::Tick(float DeltaTime)
{
	TryUpdateLoadingScreen();	
}

ETickableTickType UPIXLoadingScreenSubsystem::GetTickableTickType() const
{
	if (IsTemplate())
	{
		return ETickableTickType::Never;
	}
	
	return ETickableTickType::Conditional;
}

bool UPIXLoadingScreenSubsystem::IsTickable() const
{
	return GetGameInstance() && GetGameInstance()->GetGameViewportClient();
}

TStatId UPIXLoadingScreenSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UPIXLoadingScreenSubsystem, STATGROUP_Tickables);
}

void UPIXLoadingScreenSubsystem::OnMapPreloaded(const FWorldContext& WorldContext, const FString& MapName)
{
	
	if(WorldContext.OwningGameInstance != GetGameInstance())
	{
		return;
	}
	
	SetTickableTickType(ETickableTickType::Conditional);
	bIsCurrentlyLoadingMap = true;
	
	TryUpdateLoadingScreen();
}

void UPIXLoadingScreenSubsystem::OnMapPostLoad(UWorld* LoadedWorld)
{
	//Debug::Print(TEXT("Calling OnMapPostLoad"));
	if (LoadedWorld && LoadedWorld->GetGameInstance() == GetGameInstance())
	{
		bIsCurrentlyLoadingMap = false;
	}
}

void UPIXLoadingScreenSubsystem::TryUpdateLoadingScreen()
{
	// check if there's any startup loading screen that's currently Active 
	if (IsPreLoadingScreenActive())
	{
		return;
	}
	
	
	//check if we should show the loading screen 
	if (ShouldShowLoadingScreen())
	{
		// Try display the loading Screen here 
		TryDisplayLoadingScreenIfNoon();
		OnLoadingReasonUpdated.Broadcast(CurrentLoadingReason);
	}
	else
	{
		// try to remove the current active loading screen 
		// Notify the loading screen is complete 
		
		// Disable tick 
		TryRemoveLoadingScreen();
		HoldLoadingScreenStartUpTime = -1.0f;
		SetTickableTickType(ETickableTickType::Never);
	}
}
 
bool UPIXLoadingScreenSubsystem::IsPreLoadingScreenActive() const
{
	if(FPreLoadScreenManager* PreLoadScreenManager = FPreLoadScreenManager::Get())
	{
		return PreLoadScreenManager->HasValidActivePreLoadScreen();	
	}
	
	return false;
}

bool UPIXLoadingScreenSubsystem::ShouldShowLoadingScreen()
{
	const UPIXLoadingScreenSettings* LoadingScreenSettings = GetDefault<UPIXLoadingScreenSettings>();
	if (GIsEditor && LoadingScreenSettings->bShouldLoadingScreenInEditor)
	{
		return false;
	}
	
	//Check if the object in the world need loading screen 
	if (CheckTheNeedToShowLoadingScreen())
	{
		GetGameInstance()->GetGameViewportClient()->bDisableWorldRendering = true;
		
		return true;
	}
	
	CurrentLoadingReason = TEXT("Waiting for Texture Streaming");
	
	//There no need to show the loading screen 
	GetGameInstance()->GetGameViewportClient()->bDisableWorldRendering = false;
	
	const float CurrentTime = FPlatformTime::Seconds();
	if (HoldLoadingScreenStartUpTime < 0.f)
	{
		HoldLoadingScreenStartUpTime = CurrentTime;
	}
	
	const float ElapsedTime = CurrentTime - HoldLoadingScreenStartUpTime; 
	
	if (ElapsedTime < LoadingScreenSettings->HoldLoadingScreenExtraSeconds)
	{
		return true;
	}
		
	
	return false;
}

bool UPIXLoadingScreenSubsystem::CheckTheNeedToShowLoadingScreen()
{
	if (bIsCurrentlyLoadingMap)
	{
		CurrentLoadingReason = TEXT("Loading Map");
		
		return true;
	}
	
	UWorld* OwningWorld = GetGameInstance()->GetWorld();
	if (!OwningWorld)
	{
		CurrentLoadingReason = TEXT("Initializing World");
		return true;
	}
	
	if (!OwningWorld->HasBegunPlay())
	{
		CurrentLoadingReason = TEXT("World Hasn't Begun Play");
		return true;
	}

	if (!OwningWorld->GetFirstPlayerController())
	{
		 CurrentLoadingReason = TEXT("World Hasn't First Player Controller");
	}
	
	
	return false;
}

void UPIXLoadingScreenSubsystem::TryDisplayLoadingScreenIfNoon()
{
	//If there's already active loading Screen, return early if yes 
	if (CachedCreatedLoadingScreenWidget)
	{
		return;
	}
	
	const UPIXLoadingScreenSettings* LoadingScreenSettings = GetDefault<UPIXLoadingScreenSettings>();
	
	TSubclassOf<UUserWidget> LoadedWidgetClass = LoadingScreenSettings->GetLoadingScreenWidgetClassChecked();
	
	UUserWidget* CreatedWidget = UUserWidget::CreateWidgetInstance(*GetGameInstance(), LoadedWidgetClass, NAME_None);
	
	check(CreatedWidget);
	
	CachedCreatedLoadingScreenWidget = CreatedWidget->TakeWidget();
	
	GetGameInstance()->GetGameViewportClient()->AddViewportWidgetContent(
		CachedCreatedLoadingScreenWidget.ToSharedRef(),
		1000
	);
	
	
}

void UPIXLoadingScreenSubsystem::TryRemoveLoadingScreen()
{
	if (!CachedCreatedLoadingScreenWidget)
	{
		return;
	}
	GetGameInstance()->GetGameViewportClient()->RemoveViewportWidgetContent(CachedCreatedLoadingScreenWidget.ToSharedRef());
	CachedCreatedLoadingScreenWidget.Reset();
}
