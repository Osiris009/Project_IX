// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "PIXAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_IX_API UPIXAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:

	UPIXAbilitySystemComponent();

	// Called once abilities have been granted and the ASC is fully initialized.
  // You'll add more initialization logic here in later sections.
	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;


};
