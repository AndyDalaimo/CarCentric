// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "CarCentricCharacter.h"
#include "MyGameInstance.h"
#include "GameFramework/Actor.h"
#include "SpeedBoostPowerup.generated.h"

UCLASS()
class CARCENTRIC_API ASpeedBoostPowerup : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* PowerupMesh;

	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	// class USkeletalAnimationComponent* BirdAnimation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BoxCollider;

public:	
	// Sets default values for this actor's properties
	ASpeedBoostPowerup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Overlap Event When Player is hit by vehicle 
	UFUNCTION()
		void PowerupPlayerOnCollision(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

public:	
	// Called every frame
	// virtual void Tick(float DeltaTime) override;
private:

	// Reference to Game Instance for in game timer
	UMyGameInstance* GameInstanceRef;

	// Reference to Player for collision and speed boost interaction
	ACarCentricCharacter* PlayerRef;
};
