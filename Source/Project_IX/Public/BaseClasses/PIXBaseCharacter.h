// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PIXBaseCharacter.generated.h"

UCLASS()
class PROJECT_IX_API APIXBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APIXBaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


};
