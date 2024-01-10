// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"
#include "CarCentricCharacter.h"
#include "MyGameInstance.h"
#include "SpeedBoostPowerup.h"
#include <Kismet/GameplayStatics.h>
#include "Vehicle.generated.h"



UENUM(BlueprintType)
enum class EVehicleType : uint8 {
	DEFAULT = 0 UMETA(DisplayName = "Default"),
	COMPACT = 1 UMETA(DisplayName = "Compact"),
	TRUCK = 2 UMETA(DisplayName = "Truck")
};


UCLASS()
class CARCENTRIC_API AVehicle : public AActor
{
	GENERATED_BODY()

	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	// class UStaticMeshComponent* CarMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* CarMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BoxCollider;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = VehiclePath, meta = (AllowPrivateAccess = "true"))
	class USplineComponent* VehiclePath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Vehicle, meta = (AllowPrivateAccess = "true"))
		EVehicleType Type;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Vehicle, meta = (AllowPrivateAccess = "true"))
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Vehicle, meta = (AllowPrivateAccess = "true"))
	float Speed;

	UPROPERTY()
	FVector currentLocation;

	UPROPERTY()
	FVector goalLocation;

public:	
	// Sets default values for this actor's properties
	AVehicle();
	~AVehicle() override;

	UPROPERTY()
		bool SpeedBoostActive;

	// Reference to Speed Boost Powerup
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Vehicle, meta = (AllowPrivateAccess = "true"))
		ASpeedBoostPowerup* SpeedBoostRef;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Initialize new Vehicle
	UFUNCTION(BlueprintCallable)
		EVehicleType TypeInit();

	// Overlap Event When Player is hit by vehicle 
	UFUNCTION()
	void DamagePlayerOnCollision(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	// Event Called to subtract HP from player
	UFUNCTION(BlueprintNativeEvent)
	void DamagePlayer(float damageAmount);
	void DamagePlayer_Implementation(float damageAmount);


	// Set Vehicle Speed depending on vehicle type
	// Speed used in Timer function for movement
	float SetSpeed(EVehicleType type);

	// Set Vehicle damage depending on vehicle type
	// Damage used in Collision function to decrease Player HP
	int32 SetDamage(EVehicleType type);

	// Return True if a speed boost is spawned, to update it's location along spline
	bool CreateSpeedBoost();

private:

	// Reference to Player
	ACarCentricCharacter* PlayerRef;

	// Reference to Game Instance for in game timer
	UMyGameInstance* GameInstanceRef;

	FActorSpawnParameters SpawnInfo;

	// Timer Properties
	FTimerHandle MovementHandler;
	FTimerDelegate MovementDelegate;
	float MovementTime;

	// References to Meshes assigned to Root Component depending on Vehicle Type
	USkeletalMesh* SmallCar;
	USkeletalMesh* MediumCar;
	USkeletalMesh* LargeCar;

	// Timer to call movement of Vehicle 
	UFUNCTION()
	void MovementTimer(float movementSpeed);

public:	
	// Called every frame
	// virtual void Tick(float DeltaTime) override;

};
