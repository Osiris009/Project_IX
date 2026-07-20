      // Fill out your copyright notice in the Description page of Project Settings.


#include "BaseClasses/BasePlayerCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/InputComponent.h"
#include "AbilityClasses/PIXAbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"
#include "AbilityClasses/PIXPlayerState.h"
#include "AbilityClasses/AttributSets/AS_CharacterAttributs.h"




ABasePlayerCharacter::ABasePlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create the Ability System Component
	

	// Replication Mode:
	// - Full:    Replicate all GEs to all clients (for player-controlled characters)
	// - Mixed:   Only replicate GEs to owning client (used with PlayerState ASC pattern)
	// - Minimal: Don't replicate GEs to any clients (for AI, minions)
	
}

UAbilitySystemComponent* ABasePlayerCharacter::GetAbilitySystemComponent() const
{
	return PIXAbilitySystemComponent;
}

// Called on the SERVER when a controller possesses this character
void ABasePlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	// Server GAS init
	InitGas();
	// Grant abilities, but only on the server
	GrantDefaultAbilities();
}


// Called on the OWNING CLIENT when PlayerState replicates down
void ABasePlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	// Client GAS init
	InitGas();
}
  


void ABasePlayerCharacter::InitGas()
{
	APIXPlayerState* PS = GetPlayerState<APIXPlayerState>();
	if (!PS) return;

	// Cache the pointer to the ASC from PlayerState
	PIXAbilitySystemComponent = PS->GetPIXAbilitySystemComponent();
	ChAttributeSet = PS->GetAttributeSet();

	// This call tells the ASC who owns it (PlayerState) and who
   // is the visible avatar in the world (this Character).
   // GAS uses both internally — Owner for replication,
   // Avatar for targeting and animations.
	PIXAbilitySystemComponent->InitAbilityActorInfo(PS, this);

}

void ABasePlayerCharacter::GrantDefaultAbilities()
{
	// Only grant on server — GAS replicates ability specs to clients
	if (!HasAuthority() || !PIXAbilitySystemComponent) return;


	for (const TSubclassOf<UGameplayAbility>& AbilityClass : DefaultAbilities)
	{
		if (!AbilityClass) continue;

		// FGameplayAbilitySpec is a handle to an ability instance.
		// It stores the class, level, and how it was activated.
		// Binding input is done here via the InputID parameter.
		FGameplayAbilitySpec AbilitySpec(AbilityClass, 1);
		PIXAbilitySystemComponent->GiveAbility(AbilitySpec);
	}
}

// Called when the game starts or when spawned
void ABasePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	if(APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultInputMappingContext, 0);
		}
	}

	
}

void ABasePlayerCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ABasePlayerCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ABasePlayerCharacter::Input_Attack_Pressed()
{
	if (!PIXAbilitySystemComponent) return;

	// Activate by tag — this is the preferred AAA pattern.
	// You don't hardcode a class reference; you ask GAS to
	// activate whatever ability has this tag.
	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(
		FGameplayTag::RequestGameplayTag(FName("Ability.Attack.Melee")));

	PIXAbilitySystemComponent->TryActivateAbilitiesByTag(TagContainer);
}

// Called every frame
void ABasePlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABasePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if(UEnhancedInputComponent* EIC = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABasePlayerCharacter::Move);
		EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABasePlayerCharacter::Look);	

		// Jumping
		EIC->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EIC->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		EIC->BindAction(AttackInputAction, ETriggerEvent::Triggered,
			this, &ABasePlayerCharacter	::Input_Attack_Pressed);

	}

}

