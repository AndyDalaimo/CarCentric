// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyGameInstance.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"
#include "CarCentricCharacter.generated.h"


UCLASS(config=Game)
class ACarCentricCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Timer Test Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* TimerAction;

	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Grid, meta = (AllowPrivateAccess = "true"))
	uint8 currentDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Powerup, meta = (AllowPrivateACcess = "true"))
		float TimeIncrease;

public:
	ACarCentricCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HP, meta = (AllowPrivateAccess = "true"))
	int32 HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HP, meta = (AllowPrivateAccess = "true"))
	float GlobalTime;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
		

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

private: 
	
	// Reference to Game Instance
	UMyGameInstance* GameInstanceRef;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	// Getter and Setter for the Current direction of player movement along level
	void SetCurrentDirection(uint8 dir);
	uint8 GetCurrentDirection();

	// Called in Vehicle on damage player. Take indicated amount of damage, 
	// dependant on Vehicle Type.

	UFUNCTION()
	void PlayerDamaged(int32 damage);

	UFUNCTION()
	void PlayerHitByVehicle();

	// Update Player properties depending on type of powerup collected in world

	UFUNCTION(BlueprintCallable)
	void PlayerHealed(int32 heal);
	
	UFUNCTION(BlueprintCallable)
	void CollectPowerup(int32 damage, int32 heal);

	UFUNCTION(BlueprintCallable)
	void SpeedBoost();

};

