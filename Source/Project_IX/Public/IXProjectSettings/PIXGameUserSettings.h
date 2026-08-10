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
	
	UFUNCTION()
	bool GetAllowBackgroundAudio() const { return bAllowBackgroundAudio;}

	UFUNCTION()
	void SetAllowBackgroundAudio(bool bIsAllowed);
	
	UFUNCTION()
	bool GetUseHDRAudioMode() const { return bUseHDRAudioMode;}

	UFUNCTION()
	void SetUseHDRAudioMode(bool bIsAllowed);
	
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
	
	UPROPERTY(Config)
	bool bAllowBackgroundAudio;
	
	UPROPERTY(Config)
	bool bUseHDRAudioMode;
	//*** Audio Collection Tabs ***//
	
};
