// Fill out your copyright notice in the Description page of Project Settings.

// Populate Grid with vehicles and Time Powerups 
// Amount of grids in scene handled by Spawn Handler

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SplineComponent.h"
#include "GridTemplate.generated.h"


// Enumerator which will tell Spawn handler where Player is headed and 
// where to place its spawning collider
UENUM(BlueprintType)
enum class EGridDirection : uint8 {
	FORWARD = 0 UMETA(DisplayName = "Forward"),
	RIGHT = 1 UMETA(DisplayName = "Right"),
	LEFT = 2 UMETA(DisplayName = "Left")
};


// Dynamic layout for GridTemplate
// Default / Initialized position Will be forward 
USTRUCT(BlueprintType)
struct FGridLayout
{
	GENERATED_USTRUCT_BODY()

public:
	FGridLayout();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Properties")
	EGridDirection Direction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Properties")
	FVector splineLocation_0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Properties")
	FVector splineLocation_1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Properties")
	FRotator splineRotation_0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Properties")
	FRotator splineRotation_1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Properties")
	FVector PowerupPlacement;

	EGridDirection GrabRandomDirection();

private:
	

public:
	// Default Values
	void init()
	{
		Direction = GrabRandomDirection();
		// Direction = EGridDirection::FORWARD;
		splineLocation_0 = FVector(25.f, 0.f, 0.f);
		splineLocation_1 = FVector(75.f, 0.f, 0.f);
		splineRotation_0 = FRotator(0, 90, 0);
		splineRotation_1 = FRotator(0, 90, 0); 
		PowerupPlacement = FVector(25.f, 50.f, 0.f);
	}

};

UCLASS()
class CARCENTRIC_API AGridTemplate : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Grid, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* GridMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = VehiclePath, meta = (AllowPrivateAccess = "true"))
	class USplineComponent* VehiclePath;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = VehiclePath, meta = (AllowPrivateAccess = "true"))
	class USplineComponent* VehiclePath1;

	
public:	
	// Sets default values for this actor's properties
	AGridTemplate();
	// ~AGridTemplate();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GridProperties, meta = (AllowPrivateAccess = "true"))
		FGridLayout Layout;
};
