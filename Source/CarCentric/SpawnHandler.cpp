// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnHandler.h"

// Sets default values
ASpawnHandler::ASpawnHandler() : Rotation(0.0, 0.0, 0.0), CurrentGridIndex(0), PreviousGridIndex(0)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> SmokeParticleSystem(TEXT("/Game/StarterContent/Particles/P_Steam_Lit"));
	SmokeParticle = SmokeParticleSystem.Object;

	if (SmokeParticleSystem.Succeeded())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Found Particle System");
	}

	SpawnCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("North_BoxCollider"));
	SpawnCollider->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	SpawnCollider->SetRelativeScale3D(FVector(0.5f, 30.f, 10.f));
	SpawnCollider->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	SpawnCollider->SetVisibility(true);

	SmokeWall_0 = CreateDefaultSubobject<USplineComponent>(TEXT("Smoke Wall 1"));
	SmokeWall_0->AttachToComponent(SpawnCollider, FAttachmentTransformRules::KeepRelativeTransform);
	SmokeWall_0->SetRelativeLocationAndRotation(FVector(0, 22, 0), FRotator(0, 90, 0));
	SmokeWall_0->SetLocationAtSplinePoint(1, FVector(0, 20, 0), ESplineCoordinateSpace::Local, true);
	SmokeWall_0->AddSplinePoint(FVector(0, 40, 0), ESplineCoordinateSpace::Local, true);
	SmokeWall_0->AddSplinePoint(FVector(0, 60, 0), ESplineCoordinateSpace::Local, true);
	SmokeWall_0->Mobility = EComponentMobility::Movable;
	SmokeWall_0->SetHiddenInGame(false);

	SmokeWall_1 = CreateDefaultSubobject<USplineComponent>(TEXT("Smoke Wall 2"));
	SmokeWall_1->AttachToComponent(SpawnCollider, FAttachmentTransformRules::KeepRelativeTransform);
	SmokeWall_1->SetRelativeLocationAndRotation(FVector(0, -22, 0), FRotator(0, 90, 0));
	SmokeWall_1->SetLocationAtSplinePoint(1, FVector(0, 20, 0), ESplineCoordinateSpace::Local, true);
	SmokeWall_1->AddSplinePoint(FVector(0, 40, 0), ESplineCoordinateSpace::Local, true);
	SmokeWall_1->AddSplinePoint(FVector(0, 60, 0), ESplineCoordinateSpace::Local, true);
	SmokeWall_1->Mobility = EComponentMobility::Movable;
	SmokeWall_1->SetHiddenInGame(false);

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

		// Move Spawn collider to next correct position 
		SpawnCollider->SetWorldLocationAndRotation(UpdateSpawnColliderLocation(tempLoc, (uint8)ActiveGrids[PreviousGridIndex]->Layout.Direction),
			UpdateSpawnColliderRotation((uint8)ActiveGrids[PreviousGridIndex]->Layout.Direction));

		// ------------------ TODO --------------
		// Update location of both Fog Walls

		// Set Players current direction
		PlayerRef->SetCurrentDirection((uint8)ActiveGrids[CurrentGridIndex]->Layout.Direction);
		UE_LOG(LogTemp, Warning, TEXT("Current: %d Previous: %d"), CurrentGridIndex, PreviousGridIndex);
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
			return FVector(loc.X + 900.f, tempLoc.Y, this->GetActorLocation().Z);
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
	default : 
		return FRotator(0.0, 90.0, 0.0);
	}
	return FRotator(0.0, 0.0, 0.0);
}

// Spawn All poolable grids into scene on Begin Play. Will be updated in spawn handling collision event
void ASpawnHandler::InitializeGridPool()
{
	for (int i = 0; i < 4; i++)
	{
		if (i == 0) tempLoc = FVector(1200.0f, 1200.0f, -20.f);
		else tempLoc = FVector(-1000.f, -1000.f, 0.f);
		ActiveGrids.Push(Cast<AGridTemplate>(GetWorld()->SpawnActor<AGridTemplate>(tempLoc, Rotation, SpawnInfo)));

		// Set up Particle Emitters
		UGameplayStatics::SpawnEmitterAttached(SmokeParticle, SmokeWall_0, SmokeWall_0->GetFName(),
			FVector(SmokeWall_0->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local)), FRotator(0, 0, -10), FVector(3, 5, 3),
			EAttachLocation::SnapToTarget, false);
		UGameplayStatics::SpawnEmitterAttached(SmokeParticle, SmokeWall_1, SmokeWall_1->GetFName(),
			FVector(SmokeWall_0->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local)), FRotator(0, 0, -10), FVector(3, 5, 3),
			EAttachLocation::SnapToTarget, false);
	}
	ActiveGrids[CurrentGridIndex]->Layout.Direction = EGridDirection::FORWARD;
}

// Update ActiveGrid Index
void ASpawnHandler::UpdateIndex()
{
	CurrentGridIndex++;
	PreviousGridIndex = CurrentGridIndex - 1;

	if (CurrentGridIndex == 4)
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

