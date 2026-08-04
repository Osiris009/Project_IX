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
	
	UPIXGameUserSettings();
	
	static UPIXGameUserSettings* Get();
	
	//*** GamePlay Collection Tabs ***//
	// Returns the current game difficulty setting
	UFUNCTION()
	FString GetCurrentGameDifficulty() const { return CurrentGameDifficulty; }	
	// Sets the current game difficulty setting
	UFUNCTION()
	void SetCurrentGameDifficulty(const FString& NewDifficulty) { CurrentGameDifficulty = NewDifficulty; }
	//*** GamePlay Collection Tabs ***//       

	//*** Audio Collection Tabs ***//
	
	UFUNCTION()
	float GetOverallVolume() const { return OverallVolume; }
	UFUNCTION()
	void SetOverallVolume(float InVolume);
	
	UFUNCTION()
	float GetMusicVolume() const { return MusicVolume; }
	UFUNCTION()
	void SetMusicVolume(float InVolume);
	
	UFUNCTION()
	float GetSFXVolume() const { return SFXVolume; }
	UFUNCTION()
	void SetSFXVolume(float InVolume);
	
private:
	//*** GamePlay Collection Tabs ***//
	UPROPERTY(Config)
	FString CurrentGameDifficulty; 
	
	//*** Audio Collection Tabs ***//
	UPROPERTY(Config)
	float OverallVolume;
	UPROPERTY(Config)
	float MusicVolume;
	UPROPERTY(Config)
	float SFXVolume;
	//*** Audio Collection Tabs ***//
	
};
