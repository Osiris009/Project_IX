// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "PIXGameUserSettings.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_IX_API UPIXGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()
public:
	
	static UPIXGameUserSettings* Get();
	
	//*** GamePlay Collection Tabs ***//
	// Returns the current game difficulty setting
	UFUNCTION()
	FString GetCurrentGameDifficulty() const { return CurrentGameDifficulty; }	
	// Sets the current game difficulty setting
	UFUNCTION()
	void SetCurrentGameDifficulty(const FString& NewDifficulty) { CurrentGameDifficulty = NewDifficulty; }

	//*** GamePlay Collection Tabs ***//       

private:
	
	UPROPERTY(Config)
	FString CurrentGameDifficulty; 
};
