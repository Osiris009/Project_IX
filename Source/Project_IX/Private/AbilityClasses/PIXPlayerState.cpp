// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityClasses/PIXPlayerState.h"
#include "AbilityClasses/PIXAbilitySystemComponent.h"
#include "AbilityClasses/AttributSets/AS_CharacterAttributs.h"


APIXPlayerState::APIXPlayerState()
{
	// Create the ASC as a subobject — it will replicate automatically

	PIXAbilitySystemComponent = CreateDefaultSubobject<UPIXAbilitySystemComponent>(TEXT("PIXAbilitySystemComponent"));
	PIXAbilitySystemComponent->SetIsReplicated(true);

	// Create the AttributeSet — the ASC automatically discovers and registers
	// any AttributeSet subobjects found on its owner

	AttributeSet = CreateDefaultSubobject<UAS_CharacterAttributs>(TEXT("AttributeSet"));

	// PlayerState ticks frequently — we can reduce this since
	// GAS manages its own update cadence
	NetUpdateFrequency = 100.f;

}

UAbilitySystemComponent* APIXPlayerState::GetAbilitySystemComponent() const
{
	return GetPIXAbilitySystemComponent();
}

UPIXAbilitySystemComponent* APIXPlayerState::GetPIXAbilitySystemComponent() const
{
	return PIXAbilitySystemComponent;
}

UAS_CharacterAttributs* APIXPlayerState::GetAttributeSet() const
{
	return AttributeSet;
}
