


#include "AbilityClasses/GA_Attack.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

#include "GameFramework/DamageType.h"

UGA_Attack::UGA_Attack()
{
    // Net Execution Policy — determines where this ability runs
   // LocalPredicted: Client predicts activation, server confirms
   // (Best for responsive combat in multiplayer)
    NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UGA_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, 
    const FGameplayAbilityActorInfo* ActorInfo, 
    const FGameplayAbilityActivationInfo ActivationInfo, 
    const FGameplayEventData* TriggerEventData)
{
    // ALWAYS call Super or CommitAbility first
    // CommitAbility checks cost and cooldown, then "commits" the ability
    // If it fails (not enough resources, on cooldown), we end immediately
    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }
    // Perform the actual attack
    // End the ability — in a real game, you'd wait for the montage to finish
    // We'll add AbilityTasks for that in Section 4
    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}   

void UGA_Attack::EndAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    bool bReplicateEndAbility,
    bool bWasCancelled)
{
    // Clean up anything ability-specific here
    // (timers, references, etc.)

    // ALWAYS call Super::EndAbility — this is critical
    // It handles tag cleanup, replication, and internal state
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}