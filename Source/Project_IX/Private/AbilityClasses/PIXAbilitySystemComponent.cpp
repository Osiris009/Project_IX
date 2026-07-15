// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityClasses/PIXAbilitySystemComponent.h"


UPIXAbilitySystemComponent::UPIXAbilitySystemComponent()
{
	// Constructor logic here
    // GAS supports server-authoritative replication with client prediction.
    // Mixed mode is the standard for player characters — it replicates to
    // the owning client fully, and to other clients in a minimal form.
    ReplicationMode = EGameplayEffectReplicationMode::Mixed;
}
void UPIXAbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
    Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);
	// Additional initialization logic can be added here if needed
}
