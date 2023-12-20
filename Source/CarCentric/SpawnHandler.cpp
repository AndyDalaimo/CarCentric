// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnHandler.h"

// Sets default values
ASpawnHandler::ASpawnHandler() : Rotation(0.0, 0.0, 0.0)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("North_BoxCollider"));
	SpawnCollider->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	SpawnCollider->SetRelativeScale3D(FVector(0.5f, 30.f, 10.f));
	SpawnCollider->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	SpawnCollider->SetVisibility(true);

}

// Called when the game starts or when spawned
void ASpawnHandler::BeginPlay()
{
	Super::BeginPlay();

	PlayerRef = Cast<ACarCentricCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

	SpawnCollider->OnComponentEndOverlap.AddDynamic(this, &ASpawnHandler::SpawnGridOnCollision);

	// Collider placed in position on Grid template to Begin Play
	SpawnCollider->SetWorldLocation(FVector(1000.f, tempLoc.Y + 1000.f, tempLoc.Z + 50.f));
}

// ---------------------------------------------------------------------------------------------------------------
// -------------------------------------- Grid Spawning / Generation ---------------------------------------------
// ---------------------------------------------------------------------------------------------------------------
// Spawn a new Grid template when plaeyr passes spawn collider. Move Collider to Further position on Grid
// tempLoc will take previous Grid Template and add the size of grid to vector to place new Grid in line
// Collider will move in position onto new Grid Template
void ASpawnHandler::SpawnGridOnCollision(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if ((OtherActor == PlayerRef) && OtherComp)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Spawn new Grid Template"));

		// Will Only be called once. First grid Spawned into Level
		if (ActiveGrids.Num() == 0)
		{
			tempLoc = FVector(0.0f, 0.0f, -50.f);
			NewGrid = Cast<AGridTemplate>(GetWorld()->SpawnActor<AGridTemplate>(tempLoc, Rotation, SpawnInfo));
			NewGrid->Layout.Direction = EGridDirection::FORWARD;
			// Initialize new Grid
			// NewGrid->Init();

			ActiveGrids.Insert(NewGrid, 0);

			// Move Spawn collider to next correct position 
			SpawnCollider->SetWorldLocationAndRotation(UpdateSpawnColliderLocation(tempLoc, (uint8)NewGrid->Layout.Direction),
				UpdateSpawnColliderRotation((uint8)NewGrid->Layout.Direction));
			// return;
		}

		// Properties of New Grid Spawn Location / Rotation
		tempLoc = ActiveGrids[0]->GetActorLocation();
		
		NewGrid = Cast<AGridTemplate>(GetWorld()->SpawnActor<AGridTemplate>(UpdateGridSpawnLocation((uint8)ActiveGrids[0]->Layout.Direction), Rotation, SpawnInfo));

		// Initialize new Grid
		NewGrid->Init();

		// Move Spawn collider to next correct position 
		SpawnCollider->SetWorldLocationAndRotation(UpdateSpawnColliderLocation(tempLoc, (uint8)ActiveGrids[0]->Layout.Direction),
			UpdateSpawnColliderRotation((uint8)ActiveGrids[0]->Layout.Direction));
		
		// Add Newly spawned grid to beginning of vector
		ActiveGrids.Insert(NewGrid, 0);

		// Set Players current direction
		PlayerRef->SetCurrentDirection((uint8)ActiveGrids[0]->Layout.Direction);

		// Delete useless grid actors
		DeleteGrid();

	}
}

// Update Direction from tempLoc. This temporary value will be updated every time a new GridTemplate
// is spawned.
FVector ASpawnHandler::UpdateGridSpawnLocation(uint8 direction)
{
	// Change location based on Grid Direction (Forward, Right, Left)
	switch (direction)
	{
	case 0 : 
		return FVector(((float)tempLoc.X + 1500.f), tempLoc.Y, -50.f);
	case 1 : 
		return FVector(((float)tempLoc.X), tempLoc.Y + 1500.0f, -50.f);
	case 2 :
		return FVector(((float)tempLoc.X), tempLoc.Y - 1500.0f, -50.f);
	}

	return FVector();
}

// Update new position for Spawn Collider depending on which direction player is heading
FVector ASpawnHandler::UpdateSpawnColliderLocation(FVector loc, uint8 direction)
{
	// Change location based on Grid Direction (Forward, Right, Left)
	switch (direction)
	{
		case 0 :
			return FVector(loc.X + 1500.f, tempLoc.Y + 750.f, this->GetActorLocation().Z);
		case 1 : 
			return FVector(loc.X + 750.f, tempLoc.Y + 1500.f, this->GetActorLocation().Z);
		case 2 :
			return FVector(loc.X + 750.f, tempLoc.Y, this->GetActorLocation().Z);
	}

	return FVector(loc.X + 1500.f, this->GetActorLocation().Y, this->GetActorLocation().Z);
}

// Update new rotation for Spawn Collider depending on which direction player is heading
FRotator ASpawnHandler::UpdateSpawnColliderRotation(uint8 direction)
{
	switch (direction)
	{
	case 0  : 
		return FRotator(0.0, 0.0, 0.0);
	default : 
		return FRotator(0.0, 90.0, 0.0);
	}
	return FRotator(0.0, 0.0, 0.0);
}

// After more than 4 Grid templates have spawned, delete actor in 0 position.
void ASpawnHandler::DeleteGrid()
{
	UE_LOG(LogTemp, Warning, TEXT("%d Grid Tiles in Scene"), ActiveGrids.Num());

	if (ActiveGrids.Num() > 3)
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

