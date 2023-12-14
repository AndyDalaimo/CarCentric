// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/ConstructorHelpers.h"
#include "Powerup.generated.h"

// Enumerator which will tell which Powerup type to initiate
UENUM(BlueprintType)
enum class EPowerupType : uint8 {
	TIME = 0 UMETA(DisplayName = "Time"),
	LRGTIME = 1 UMETA(DisplayName = "LargeTime"),
	HEALTH = 2 UMETA(DisplayName = "Health")
};

UCLASS()
class CARCENTRIC_API APowerup : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Grid, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* PowerupMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Type, meta = (AllowPrivateAccess = "true"))
		EPowerupType Type;

public:	
	// Sets default values for this actor's properties
	APowerup();

	UFUNCTION()
		void init();

	// Time added to game clock on Collection 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 TimeIncrease;

	// HP given by powerup on player collection
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 HP;

	// HP taken by powerup on player collection
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 Damage;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Initialize random type
	EPowerupType SetRandomType();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// Assets to assign to correct type powerup
	UStaticMesh* Cone;
	UStaticMesh* Cube;
	UStaticMesh* BlueCube;

};
