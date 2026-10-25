// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PIXLoadingScreenSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_IX_API UPIXLoadingScreenSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()
	
public:	
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoadingReasonUpdatedDelegate, const FString&, CureentLoadingReason);
	
	UPROPERTY(BlueprintAssignable)
	FOnLoadingReasonUpdatedDelegate OnLoadingReasonUpdated;
	
	virtual bool ShouldCreateSubsystem(UObject* Outer) const;

	/** Implement this for initialization of instances of the system */
	virtual void Initialize(FSubsystemCollectionBase& Collection) ;

	/** Implement this for deinitialization of instances of the system */
	virtual void Deinitialize();

	//~ FTickableGameObject
	virtual UWorld* GetTickableGameObjectWorld() const  override;
	virtual void Tick( float DeltaTime )override;
	virtual ETickableTickType GetTickableTickType() const override;
	virtual bool IsTickable() const override;
	virtual TStatId GetStatId() const override;
	//~ end FTickableGameObject
	
	
private:
	
	void OnMapPreloaded(const FWorldContext& WorldContext, const FString& MapName);
	
	void OnMapPostLoad(UWorld* LoadedWorld);
	
	void TryUpdateLoadingScreen();
	bool bIsCurrentlyLoadingMap = false;
	
	bool IsPreLoadingScreenActive() const;
	
	bool ShouldShowLoadingScreen();
	
	bool CheckTheNeedToShowLoadingScreen();
	
	float HoldLoadingScreenStartUpTime = -1.0f;
	
	FString CurrentLoadingReason; 
};
