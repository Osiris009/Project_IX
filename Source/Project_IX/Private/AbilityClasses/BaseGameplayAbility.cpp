// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityClasses/BaseGameplayAbility.h"
#include "AbilityClasses/PIXAbilitySystemComponent.h"


UBaseGameplayAbility::UBaseGameplayAbility()
{
    // Default instancing policy — we'll discuss this in detail in Section 4
   // InstancedPerActor means one instance of this ability exists per character
    // InstancingPolicy defines whether the ability creates a new UObject
    // each time it's activated, or reuses a single one.
    //
    // NonInstanced:          One shared object — no per-activation state. Fastest.
    //                        Rarely used; can't use latent AbilityTasks.
    // InstancedPerActor:     One object per character. Persists between activations.
    //                        Good for most abilities. THIS is the standard default.
    // InstancedPerExecution: New object every activation. Most flexible, most memory.
    //                        Use only when you need truly isolated per-cast state.

    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

UPIXAbilitySystemComponent* UBaseGameplayAbility::GetPIXAbilitySystemComponent() const
{
    // Get the actor info for this ability
   
    // Cast the AbilitySystemComponent to our custom type
    return Cast<UPIXAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
}

