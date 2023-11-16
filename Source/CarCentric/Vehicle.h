// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "CarCentricCharacter.h"
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* CarMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BoxCollider;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Vehicle, meta = (AllowPrivateAccess = "true"))
	int32 Damage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Vehicle, meta = (AllowPrivateAccess = "true"))
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Vehicle, meta = (AllowPrivateAccess = "true"))
	EVehicleType Type;

	 
	
public:	
	// Sets default values for this actor's properties
	AVehicle();
	~AVehicle() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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
	void DamagePlayer(int32 damageAmount);
	void DamagePlayer_Implementation(int32 damageAmount);


	// Set Vehicle Speed depending on vehicle type
	// Speed used in Timer function for movement
	float SetSpeed(EVehicleType type);

private:

	// Reference to Player
	ACarCentricCharacter* PlayerRef;

	// Timer Properties
	FTimerHandle MovementHandler;
	FTimerDelegate MovementDelegate;
	float MovementTime;

	// Timer to call movement of Vehicle 
	UFUNCTION()
	void MovementTimer(float movementSpeed);

public:	
	// Called every frame
	// virtual void Tick(float DeltaTime) override;

};
