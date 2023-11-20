// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

	UPROPERTY()
		uint8 CurrentGridDirection;

	
public:	
	// Sets default values for this actor's properties
	ASpawnHandler();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	FVector tempLoc = FVector(0.0f, 0.0f, 0.0f);

	// Grid template to Spawn
	AGridTemplate* NewGrid;

	// Spawn new Grid Template 
	UFUNCTION()
		void SpawnGridOnCollision(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		FVector UpdateGridSpawnLocation(uint8 direction);

	UFUNCTION()
		FVector UpdateSpawnColliderLocation(FVector loc, uint8 direction);

	UFUNCTION()
		FRotator UpdateSpawnColliderRotation(uint8 direction);

	// Delete oldest grid from World and update array
	UFUNCTION()
		void DeleteGrid();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
