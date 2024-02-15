// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CarCentricCharacter.h"
#include "MyGameInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/BoxComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "LargeTimePowerup.generated.h"

UCLASS()
class CARCENTRIC_API ALargeTimePowerup : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* PowerupMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* BoxCollider;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		class URotatingMovementComponent* RotateComp;

public:	
	// Sets default values for this actor's properties
	ALargeTimePowerup();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Overlap Event when Player Collects powerup
	UFUNCTION()
		void CollectPowerup(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// Reference to Player
	ACarCentricCharacter* PlayerRef;

	// Reference to Game Instance (For Updating timer)
	UMyGameInstance* GameInstanceRef;

	// Amount to increase timer in Game Instance
	int32 timeIncrease;

	// Amount to damage Player 
	int32 damage;

};
