// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityClasses/AttributSets/AS_CharacterAttributs.h"
#include "Net/UnrealNetwork.h" // For Replication

UAS_CharacterAttributs::UAS_CharacterAttributs()
{
	// Sensible defaults — real values typically come from a
   // GameplayEffect applied at spawn (we'll do this properly in 3.x Part 2)

	InitHealth(100.f);
	InitMaxHealth(100.f);
	InitStamina(100.f);
	InitMaxStamina(100.f);
}

void UAS_CharacterAttributs::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// COND_None means: replicate to everyone who can see this actor.
	// REPNOTIFY_Always means: fire the OnRep even if the value
	// didn't visibly change (important — see explanation below).

	DOREPLIFETIME_CONDITION_NOTIFY(UAS_CharacterAttributs, Health, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UAS_CharacterAttributs, MaxHealth, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UAS_CharacterAttributs, Stamina, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UAS_CharacterAttributs, MaxStamina, COND_None, REPNOTIFY_Always);


}

void UAS_CharacterAttributs::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	// Clamp CurrentValue here. This runs for BOTH permanent changes
	// and temporary modifier changes — it's the universal safety net.

	if(Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	
	else if(Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxStamina());
	}
	
}

void UAS_CharacterAttributs::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{

	Super::PostGameplayEffectExecute(Data);
	// We'll build this out fully in Section 3 Part 2 and Section 7
   // (this is where damage->death logic and UI broadcast hooks live)
}

void UAS_CharacterAttributs::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAS_CharacterAttributs, Health, OldHealth);
}

void UAS_CharacterAttributs::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAS_CharacterAttributs, MaxHealth, OldMaxHealth);
}

void UAS_CharacterAttributs::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAS_CharacterAttributs, Stamina, OldStamina);	
}

void UAS_CharacterAttributs::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAS_CharacterAttributs, MaxStamina, OldMaxStamina);
}

