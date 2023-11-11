// Fill out your copyright notice in the Description page of Project Settings.

// Populate Grid with vehicles and Time Powerups 
// Amount of grids in scene handled by Spawn Handler

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridTemplate.generated.h"

UCLASS()
class CARCENTRIC_API AGridTemplate : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Grid, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* GridMesh;




	
public:	
	// Sets default values for this actor's properties
	AGridTemplate();
	~AGridTemplate() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
