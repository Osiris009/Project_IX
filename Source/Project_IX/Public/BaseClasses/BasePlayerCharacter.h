// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "AbilitySystemInterface.h"  
#include "BasePlayerCharacter.generated.h"


class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;

class UPIXAbilitySystemComponent;
class UAS_CharacterAttributs;
class UGameplayAbility;
class UGameplayEffect;


UENUM(BlueprintType)
enum class EGaitState : uint8
{
	Walk       UMETA(DisplayName = "Walk"),
	Run        UMETA(DisplayName = "Run"),
	Sprint     UMETA(DisplayName = "Sprint")
};


UCLASS()
class PROJECT_IX_API ABasePlayerCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABasePlayerCharacter();
	

	//~ Begin IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~ End IAbilitySystemInterface

	// Called by the Character to hand its identity to the ASC.
	// The ASC needs two actors: the Owner (PlayerState) and the Avatar (the visible Character).
	virtual void PossessedBy(AController* NewController) override;       // Server
	virtual void OnRep_PlayerState() override;                           // Client


	//~Inputs 
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* DefaultInputMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Sprint Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

	/** Attack Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AttackAction;


	//~Inputs

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	// The ability system component — the brain of GAS for this actor
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<UPIXAbilitySystemComponent> PIXAbilitySystemComponent;
	
	// The attribute set — we'll create this in Section 3
	UPROPERTY()
	TObjectPtr<UAS_CharacterAttributs> ChAttributeSet;

	// Abilities to grant on spawn
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;
	
	// Effects to apply on spawn (e.g., set initial attribute values)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS|Effects")
	TArray<TSubclassOf<UGameplayEffect>> DefaultEffects;

	// Called on both server and client once PlayerState is valid
	virtual void InitGas();

	// Grant all abilities in DefaultAbilities to the ASC
	virtual void GrantDefaultAbilities();


	//Movement state	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gait")
	EGaitState GaitState;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	


};
