// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnHandler.h"

// Sets default values
ASpawnHandler::ASpawnHandler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	North_BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("North_BoxCollider"));
	North_BoxCollider->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	North_BoxCollider->SetRelativeScale3D(FVector(0.5f, 25.f, 1.f));
	North_BoxCollider->SetVisibility(true);

	
	West_BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("West_BoxCollider"));
	West_BoxCollider->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	West_BoxCollider->SetRelativeScale3D(FVector(25.f, .5f, 1.f));
	West_BoxCollider->SetVisibility(true);


	East_BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("East_BoxCollider"));
	East_BoxCollider->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	East_BoxCollider->SetRelativeScale3D(FVector(25.f, .5f, 1.f));
	East_BoxCollider->SetVisibility(true);


}

// Called when the game starts or when spawned
void ASpawnHandler::BeginPlay()
{
	Super::BeginPlay();

	// Set player Ref
	PlayerRef = StaticCast<ACarCentricCharacter*>(GetWorld()->GetFirstPlayerController()->GetPawn());
	UE_LOG(LogTemp, Warning, TEXT("Player Location: %s"), *PlayerRef->GetActorLocation().ToString());
	
	// Grab Player Location and place spawner in correct position
	FVector PlayerLoc = PlayerRef->GetActorLocation();
	North_BoxCollider->SetWorldLocation(FVector(PlayerLoc.X + 1500.f, PlayerLoc.Y, PlayerLoc.Z));
	West_BoxCollider->SetWorldLocation(FVector(PlayerLoc.X, PlayerLoc.Y - 1500.f, PlayerLoc.Z));
	East_BoxCollider->SetWorldLocation(FVector(PlayerLoc.X, PlayerLoc.Y + 1500.f, PlayerLoc.Z));
}

// Called every frame
void ASpawnHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

