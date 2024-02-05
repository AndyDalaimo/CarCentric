// Copyright Epic Games, Inc. All Rights Reserved.

#include "CarCentricCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


//////////////////////////////////////////////////////////////////////////
// ACarCentricCharacter

ACarCentricCharacter::ACarCentricCharacter() : HP(50.f), currentDirection(0), GlobalTime(1.f), TimeIncrease(.2f)
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 850.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm


	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}


void ACarCentricCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// Binding function for timer to play out on Player Death
	TimerDelegate.BindUFunction(this, "PlayerDead");

	// Set reference to Game Instance
	GameInstanceRef = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void ACarCentricCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACarCentricCharacter::Move);

		//Looking
		// EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACarCentricCharacter::Look);

	}

}



void ACarCentricCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();
	
	if (Controller != nullptr && GetCharacterMovement()->IsMovingOnGround())
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

/*void ACarCentricCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}*/


//////////////////////////////////////////////////////////////////////////
// Spawn generation attributes


// Set the current direction that the player is traveling.
// If player is traveling right, grid cannot spawn to the left
// and vice-a-versa. This avoids back tracking.
void ACarCentricCharacter::SetCurrentDirection(uint8 dir)
{
	currentDirection = dir;
}

// Get current direcrion Player is traveling in level
uint8 ACarCentricCharacter::GetCurrentDirection()
{
	return currentDirection;
}

//////////////////////////////////////////////////////////////////////////
// Player interaction with World Items (pickups / vehicles)

// Damage taken from Vehicle. Amoutn dependant on vehicle type. 
// If HP <= 0, call Game Over in Game Instance
void ACarCentricCharacter::PlayerDamaged(float damage)
{
	if (HP - damage <= 0) HP = 0;
	else HP -= damage;

	// Call Screen shake event in player Blueprint
	ScreenShake();

	// On Player Death
	if (HP <= 0)
	{
		GameInstanceRef->ShowGameOverUIWidget();
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), .8f);
		GetWorld()->GetTimerManager().SetTimer(Timer, TimerDelegate, 0.05f, true);
	}
}

// Timer Bound function when HP reaches zero.
// Camera to zoom in on Player Death -> For epic dramatic effect. 
void ACarCentricCharacter::PlayerDead()
{
	CameraBoom->TargetArmLength -= 5.f;
	if (CameraBoom->TargetArmLength == 500.f)
	{
		GetWorld()->GetTimerManager().ClearTimer(Timer);
		// GameInstanceRef->ShowGameOverUIWidget();
	}
}

// Add Small impulse to player when hit by a vehicle. Reset currentSpeed to baseSpeed
void ACarCentricCharacter::PlayerHitByVehicle()
{
	if (GetCharacterMovement()->IsMovingOnGround())
		GetCharacterMovement()->AddImpulse(GetActorForwardVector() * -7000, true);
	else GetCharacterMovement()->AddImpulse(GetActorForwardVector() * -1800, true);
	
	// currentSpeed = baseSpeed;
	// GetCharacterMovement()->MaxWalkSpeed = currentSpeed;
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
	GlobalTime = 1.0f;
	GameInstanceRef->SetTimerRate(GlobalTime);
}

// Health pickups in world will heal player 
// If HP at max (No Effect)
void ACarCentricCharacter::PlayerHealed(int32 heal)
{
	if (HP < 50) HP += heal;
}

// Increase Player's max walk speed on powerup collection
void ACarCentricCharacter::SpeedBoost()
{
	GlobalTime = UGameplayStatics::GetGlobalTimeDilation(GetWorld());

	if (GlobalTime < 2.f)
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), GlobalTime += TimeIncrease);
		GameInstanceRef->SetTimerRate(GlobalTime);
		// Play UI Animation in Player blueprint
		UISpeedIndication();
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::SanitizeFloat(GlobalTime));
	
}

// Called on any powerup. Will add to values appropriately 
void ACarCentricCharacter::CollectPowerup(int32 damage, int32 heal)
{
}



