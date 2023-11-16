// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridTemplate.h"
#include "CarCentricCharacter.h"
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

	
public:	
	// Sets default values for this actor's properties
	ASpawnHandler();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	FVector tempLoc = FVector(0.0f, 0.0f, 0.0f);

	// Reference to Player
	ACarCentricCharacter* PlayerRef;

	// Grid template to Spawn
	AGridTemplate* NewGrid;

	// Spawn new Grid Template 
	UFUNCTION()
		void SpawnGridOnCollision(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Delete oldest grid from World and update array
	UFUNCTION()
		void DeleteGrid();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
