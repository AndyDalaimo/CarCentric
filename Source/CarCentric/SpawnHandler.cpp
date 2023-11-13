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
	North_BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &ASpawnHandler::SpawnGridOnCollision);

	
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
	North_BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &ASpawnHandler::SpawnGridOnCollision);
	
	// Grab Player Location and place spawner in correct position
	FVector PlayerLoc = PlayerRef->GetActorLocation();
	North_BoxCollider->SetWorldLocation(FVector(PlayerLoc.X + 1100.f, PlayerLoc.Y, PlayerLoc.Z));
	West_BoxCollider->SetWorldLocation(FVector(PlayerLoc.X, PlayerLoc.Y - 1500.f, PlayerLoc.Z));
	East_BoxCollider->SetWorldLocation(FVector(PlayerLoc.X, PlayerLoc.Y + 1500.f, PlayerLoc.Z));
}

// Spawn a new Grid template when plaeyr passes spawn collider. Move Collider to Further position on Grid
void ASpawnHandler::SpawnGridOnCollision(UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult& SweepResult)
{
	if ((OtherActor) && (OtherActor != this) && OtherComp)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Spawn new Grid Template"));

		FVector Location(North_BoxCollider->GetComponentLocation().X + 1300.f, 0.0f, 0.0f);
		FRotator Rotation(0.0f, 0.0f, 0.0f);
		FActorSpawnParameters SpawnInfo;

		// Add Newly spawned grid to beginning of vector
		ActiveGrids.Insert((Cast<AGridTemplate>(GetWorld()->SpawnActor<AGridTemplate>(Location, Rotation, SpawnInfo))), 0);

		North_BoxCollider->SetWorldLocation(FVector(this->GetActorLocation().X + 1500.f, 
			this->GetActorLocation().Y, this->GetActorLocation().Z));

		// Delete useless grid actors
		DeleteGrid();
	}
	
}

// After more than 2 Grid templates have spawned, delete actor in 0 position.
void ASpawnHandler::DeleteGrid()
{
	UE_LOG(LogTemp, Warning, TEXT("%d Grid Tiles in Scene"), ActiveGrids.Num());

	if (ActiveGrids.Num() > 4)
	{
		// Remove Grid Actor from end of Vector (oldest element)
		GetWorld()->DestroyActor(ActiveGrids.Pop(true));

	}
	
}

// Called every frame
void ASpawnHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

