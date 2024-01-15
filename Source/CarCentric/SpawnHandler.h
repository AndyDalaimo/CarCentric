

#include "Components/BoxComponent.h"
#include "Components/BoxComponent.h"// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CarCentricCharacter.h"
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

	UPROPERTY()
		TArray<AGridTemplate*> ActiveGrids;

	// Grid Spawn Properties 
	FRotator Rotation;
	FActorSpawnParameters SpawnInfo;

public:	
	// Sets default values for this actor's properties
	ASpawnHandler();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

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
