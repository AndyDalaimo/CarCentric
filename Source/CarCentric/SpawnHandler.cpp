// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnHandler.h"

// Sets default values
ASpawnHandler::ASpawnHandler() : Rotation(0.0, 0.0, 0.0), CurrentGridIndex(0), PreviousGridIndex(0)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> SmokeParticleSystem(TEXT("/Game/StarterContent/Particles/P_Steam_Lit"));

	if (SmokeParticleSystem.Succeeded())
	{
		SmokeParticle = SmokeParticleSystem.Object;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Found Particle System"));
	}

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

	SpawnCollider->OnComponentEndOverlap.AddUniqueDynamic(this, &ASpawnHandler::SpawnGridOnCollision);

	// Collider placed in position on Grid template to Begin Play
	SpawnCollider->SetWorldLocation(FVector(1000.f, 1000.f, tempLoc.Z + 50.f));

	InitializeGridPool();
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
		UE_LOG(LogTemp, Warning, TEXT("Current Grid Direction: %d"), (uint8)ActiveGrids[CurrentGridIndex]->Layout.Direction);

		// Update Indices of temp and previous Active grids
		UpdateIndex();

		// Properties of New Grid Spawn Location / Rotation
		tempLoc = ActiveGrids[PreviousGridIndex]->GetActorLocation();
		
		// NewGrid = Cast<AGridTemplate>(GetWorld()->SpawnActor<AGridTemplate>(UpdateGridSpawnLocation((uint8)ActiveGrids[0]->Layout.Direction), Rotation, SpawnInfo));
		ActiveGrids[CurrentGridIndex]->SetActorLocationAndRotation(UpdateGridSpawnLocation((uint8)ActiveGrids[PreviousGridIndex]->Layout.Direction), Rotation);
		
		// Initialize new Grid
		ActiveGrids[CurrentGridIndex]->Layout.init(PlayerRef->GetCurrentDirection());
		ActiveGrids[CurrentGridIndex]->Init();

		ActiveGrids[CurrentGridIndex]->SetActorLocationAndRotation(UpdateGridSpawnLocation((uint8)ActiveGrids[PreviousGridIndex]->Layout.Direction), Rotation);

		// Move Spawn collider to next correct position 
		SpawnCollider->SetWorldLocationAndRotation(UpdateSpawnColliderLocation(tempLoc, (uint8)ActiveGrids[PreviousGridIndex]->Layout.Direction),
			UpdateSpawnColliderRotation((uint8)ActiveGrids[PreviousGridIndex]->Layout.Direction));

		// Update text in Blueprint
		UpdateText();

		// Set Players current direction
		PlayerRef->SetCurrentDirection((uint8)ActiveGrids[CurrentGridIndex]->Layout.Direction);// Update Indices of temp and previous Active grids

		UpdateIndex();

		// Properties of New Grid Spawn Location / Rotation
		tempLoc = ActiveGrids[PreviousGridIndex]->GetActorLocation();
		
		// NewGrid = Cast<AGridTemplate>(GetWorld()->SpawnActor<AGridTemplate>(UpdateGridSpawnLocation((uint8)ActiveGrids[0]->Layout.Direction), Rotation, SpawnInfo));
		ActiveGrids[CurrentGridIndex]->SetActorLocationAndRotation(UpdateGridSpawnLocation((uint8)ActiveGrids[PreviousGridIndex]->Layout.Direction), Rotation);
		
		// Initialize new Grid
		ActiveGrids[CurrentGridIndex]->Layout.init(PlayerRef->GetCurrentDirection());
		ActiveGrids[CurrentGridIndex]->Init();

		ActiveGrids[CurrentGridIndex]->SetActorLocationAndRotation(UpdateGridSpawnLocation((uint8)ActiveGrids[PreviousGridIndex]->Layout.Direction), Rotation);

		// Move Spawn collider to next correct position 
		// SpawnCollider->SetWorldLocationAndRotation(UpdateSpawnColliderLocation(tempLoc, (uint8)ActiveGrids[PreviousGridIndex]->Layout.Direction),
		// 	UpdateSpawnColliderRotation((uint8)ActiveGrids[PreviousGridIndex]->Layout.Direction));

		// Set Players current direction
		PlayerRef->SetCurrentDirection((uint8)ActiveGrids[CurrentGridIndex]->Layout.Direction);
		UE_LOG(LogTemp, Warning, TEXT("Current: %d Previous: %d"), CurrentGridIndex, PreviousGridIndex);
	}
}

// Change text on road sign
void ASpawnHandler::UpdateText_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Change Text"));
}

// Update Direction from tempLoc. This temporary value will be updated every time a new GridTemplate
// is spawned.
FVector ASpawnHandler::UpdateGridSpawnLocation(uint8 direction)
{
	// Change location based on Grid Direction (Forward, Right, Left)
	switch (direction)
	{
	case 0 : 
		// return FVector(((float)tempLoc.X + 1500.f), tempLoc.Y, -50.f);
		return FVector(((float)tempLoc.X + 1440.f), tempLoc.Y, -20.f);
	case 1 : 
		// return FVector(((float)tempLoc.X), tempLoc.Y + 1500.0f, -50.f);
		return FVector(((float)tempLoc.X), tempLoc.Y + 1440.0f, -20.f);
	case 2 :
		// return FVector(((float)tempLoc.X), tempLoc.Y - 1500.0f, -50.f);
		return FVector(((float)tempLoc.X), tempLoc.Y - 1440.0f, -20.f);
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
			// return FVector(loc.X + 1500.f, tempLoc.Y + 750.f, this->GetActorLocation().Z);
			return FVector(loc.X + 720.f, tempLoc.Y, this->GetActorLocation().Z);
		case 1 : 
			// return FVector(loc.X + 750.f, tempLoc.Y + 1500.f, this->GetActorLocation().Z);
			return FVector(loc.X, tempLoc.Y + 700.f, this->GetActorLocation().Z);
		case 2 :
			// return FVector(loc.X + 750.f, tempLoc.Y, this->GetActorLocation().Z);
			return FVector(loc.X, tempLoc.Y - 700.f, this->GetActorLocation().Z);
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
	case 1  : 
		return FRotator(0.0, 90.0, 0.0);
	case 2  :
		return FRotator(0.0, -90.0, 0.0);
	}
	return FRotator(0.0, 0.0, 0.0);
}

// Spawn All poolable grids into scene on Begin Play. Will be updated in spawn handling collision event
void ASpawnHandler::InitializeGridPool()
{
	for (int i = 0; i < 8; i++)
	{
		if (i == 0) tempLoc = FVector(2150.0f, 1000.0f, -20.f);
		else tempLoc = FVector(-1000.f, -1000.f, 0.f);
		ActiveGrids.Push(Cast<AGridTemplate>(GetWorld()->SpawnActor<AGridTemplate>(tempLoc, Rotation, SpawnInfo)));
	}
	ActiveGrids[CurrentGridIndex]->Layout.Direction = EGridDirection::FORWARD;
}

// Update ActiveGrid Index
void ASpawnHandler::UpdateIndex()
{
	CurrentGridIndex++;
	PreviousGridIndex = CurrentGridIndex - 1;

	if (CurrentGridIndex == 8)
	{
		CurrentGridIndex = 0;
	}
}

// NOT IN USE AFTER SETTING THE GRIDS IN OBJECT POOL
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

