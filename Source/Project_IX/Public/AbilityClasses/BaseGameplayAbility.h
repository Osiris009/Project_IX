// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "BaseGameplayAbility.generated.h"

/**
 * Base gameplay ability class for our project.
 * All project abilities should derive from this, NOT from UGameplayAbility directly.
 * This gives us a single place to add project-wide ability behavior.
 */
UCLASS()
class PROJECT_IX_API UBaseGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:

	UBaseGameplayAbility();

	// Which input action activates this ability (we'll use this for input binding)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	int32 AbilityInputID = 0;
};
