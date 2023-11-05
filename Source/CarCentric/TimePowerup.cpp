// Fill out your copyright notice in the Description page of Project Settings.


#include "TimePowerup.h"
#include "Engine/World.h"

// Sets default values
ATimePowerup::ATimePowerup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ClockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ClockMesh"));
	ClockMesh->SetupAttachment(RootComponent);

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	BoxCollider->AttachToComponent(ClockMesh, FAttachmentTransformRules::KeepRelativeTransform);
	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &ATimePowerup::CollectPowerup);


}


// Event Called when Player picks up time powerup. Will call event to update player timer and destroy object
void ATimePowerup::CollectPowerup(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Overlap Begin"));
		UpdateGameInstanceTimer();
		Destroy();
	}
}

// Blueprint Event To update in game timer
void ATimePowerup::UpdateGameInstanceTimer_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Update Game Timer"));
}

// Called when the game starts or when spawned
/*void ATimePowerup::BeginPlay()
{
	Super::BeginPlay();
	
}*/

// Called every frame
/*void ATimePowerup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}*/

