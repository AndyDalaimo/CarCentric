// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Vehicle.h"
#include "FollowSplineComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CARCENTRIC_API UFollowSplineComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY()
		AVehicle* VehicleRef;


public:	
	// Sets default values for this component's properties
	UFollowSplineComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
