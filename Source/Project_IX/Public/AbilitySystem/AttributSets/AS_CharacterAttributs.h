// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AS_CharacterAttributs.generated.h"

// This macro pair generates getter/setter/initter boilerplate for every
// attribute. Writing this by hand for every stat is tedious and error-prone,
// so GAS provides this macro. ATTRIBUTE_ACCESSORS expands to:
//   GetHealth(), GetHealthAttribute(), SetHealth(), InitHealth()

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class PROJECT_IX_API UAS_CharacterAttributs : public UAttributeSet
{
	GENERATED_BODY()
	
public:

	UAS_CharacterAttributs();


	//Replicated properties
	 // Required override — tells the engine which properties to replicate
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//Health
	UPROPERTY(BlueprintReadOnly, Category = "HealthAttributes", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health; 
	ATTRIBUTE_ACCESSORS(UAS_CharacterAttributs, Health)

	UPROPERTY(BlueprintReadOnly, Category = "HealthAttributes", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UAS_CharacterAttributs, MaxHealth)

	//Stamina
	UPROPERTY(BlueprintReadOnly, Category = "StaminaAttributes", ReplicatedUsing = OnRep_Stamina)
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UAS_CharacterAttributs, Stamina)

		UPROPERTY(BlueprintReadOnly, Category = "StaminaAttributes", ReplicatedUsing = OnRep_MaxStamina)
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UAS_CharacterAttributs, MaxStamina)

protected:
	// Critical override — this is where we CLAMP values.
   // Called BEFORE a GameplayEffect modification is applied.
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	//RepNotify functions
	 // Called AFTER a GameplayEffect has been fully applied (base value change).
	// This is where we clamp Health to MaxHealth after permanent changes,
	// and where we'll hook death detection later.S

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	UFUNCTION()//RepNotify function for Health
	void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UFUNCTION()//RepNotify function for MaxHealthS
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	UFUNCTION()//RepNotify function for Stamina
	void OnRep_Stamina(const FGameplayAttributeData& OldStamina);

	UFUNCTION()//RepNotify function for MaxStamina
		void OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina);
};
