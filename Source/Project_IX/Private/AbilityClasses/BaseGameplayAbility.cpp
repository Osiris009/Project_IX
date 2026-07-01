// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityClasses/BaseGameplayAbility.h"

UBaseGameplayAbility::UBaseGameplayAbility()
{
    // Default instancing policy — we'll discuss this in detail in Section 4
   // InstancedPerActor means one instance of this ability exists per character
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}