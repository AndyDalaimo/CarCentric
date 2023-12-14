// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CarCentricCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/BoxComponent.h"
#include "HealthPowerup.generated.h"

UCLASS()
class CARCENTRIC_API AHealthPowerup : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* HeartMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collison, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* BoxCollider;

	
public:	
	// Sets default values for this actor's properties
	AHealthPowerup();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 HP;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Overlap Event when Player Collects powerup
	UFUNCTION()
		void CollectHealthPowerup(UPrimitiveComponent* OverlappedComponent,
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

};
