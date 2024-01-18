// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CarCentricCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "ParticleDefinitions.h"
#include "Components/SplineComponent.h"
#include "Components/BoxComponent.h"
#include "GridTemplate.h"
#include "Components/BoxComponent.h"
#include "SpawnHandler.generated.h"

UCLASS()
class CARCENTRIC_API ASpawnHandler : public AActor
{
	GENERATED_BODY()

	// Box Collision at top of grid
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = SpawnCollider, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* SpawnCollider;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = VehiclePath, meta = (AllowPrivateAccess = "true"))
		class USplineComponent* SmokeWall_0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = VehiclePath, meta = (AllowPrivateAccess = "true"))
		class USplineComponent* SmokeWall_1;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = VehiclePath, meta = (AllowPrivateAccess = "true"))
		class USplineComponent* SmokeWall_2;

	UPROPERTY()
		TArray<AGridTemplate*> ActiveGrids;


	// Grid Spawn Properties 
	FRotator Rotation;
	FActorSpawnParameters SpawnInfo;

public:	
	// Sets default values for this actor's properties
	ASpawnHandler();

	// To be implemented in blueprint
	UFUNCTION(BlueprintNativeEvent)
		void UpdateText();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	// Reference to Smoke Particle system
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Particle, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* SmokeParticle;

	// Reference to Player
	ACarCentricCharacter* PlayerRef;


	FVector tempLoc = FVector(-1000.0f, -1000.0f, 0.0f);

	// Tracked current index of active grid template to move
	int8 CurrentGridIndex;
	// Tracked previous index of active grid template to move
	uint8 PreviousGridIndex;

	// Spawn new Grid Template 
	UFUNCTION()
		void SpawnGridOnCollision(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		FVector UpdateGridSpawnLocation(uint8 direction);

	UFUNCTION()
		FVector UpdateSpawnColliderLocation(FVector loc, uint8 direction);

	UFUNCTION()
		FRotator UpdateSpawnColliderRotation(uint8 direction);

	// Initialize Pooled Grids to update in level generation
	UFUNCTION()
		void InitializeGridPool();

	// Delete oldest grid from World and update array
	UFUNCTION()
		void DeleteGrid();

	UFUNCTION()
		void UpdateIndex();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
