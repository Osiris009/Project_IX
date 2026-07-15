// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h" 
#include "PIXPlayerState.generated.h"


class UPIXAbilitySystemComponent;
class UAS_CharacterAttributs;


UCLASS()
class PROJECT_IX_API APIXPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:

	APIXPlayerState();

	// IAbilitySystemInterface — GAS queries this to find the ASC on any actor.
	// Every actor that participates in GAS must implement this.
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	UPIXAbilitySystemComponent* GetPIXAbilitySystemComponent() const;

	UAS_CharacterAttributs* GetAttributeSet() const;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<UPIXAbilitySystemComponent> PIXAbilitySystemComponent;

	// AttributeSet also lives on PlayerState for the same reason
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<UAS_CharacterAttributs> AttributeSet;
};
