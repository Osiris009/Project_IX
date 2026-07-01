// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityClasses/BaseGameplayAbility.h"
#include "GA_Attack.generated.h"


UCLASS()
class PROJECT_IX_API UGA_Attack : public UBaseGameplayAbility
{
	GENERATED_BODY()
	
public:	

	UGA_Attack();

	/*** Called when the ability is activated — this is where your ability logic goes ***/
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, 
		const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, 
		const FGameplayEventData* TriggerEventData) 
		override;

	/*** Called when the ability ends — this is where you clean up any ongoing effects or state ***/
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, 
		const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, 
		bool bReplicateEndAbility, 
		bool bWasCancelled) 
		override;

};
