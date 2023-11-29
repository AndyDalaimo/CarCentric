// Fill out your copyright notice in the Description page of Project Settings.


#include "GridTemplate.h"

// Sets default values
AGridTemplate::AGridTemplate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// static ConstructorHelpers::FClassFinder<AVehicle> VehicleFinder(TEXT("/Game/WorldItems/BP_Vehicle"));
	// vehicleClass = VehicleFinder.Class;
	
	// static ConstructorHelpers::FClassFinder<ATimePowerup> PowerupFinder(TEXT("/Game/WorldItems/BP_TimePowerup"));
	// powerupClass = PowerupFinder.Class; && PowerupFinder.Succeeded()

	/*if (VehicleFinder.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("BP FOUND"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("BP NOT FOUND"));
		vehicleClass = AVehicle::StaticClass();
		// powerupClass = ATimePowerup::StaticClass();
	}*/


	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("/Game/LevelPrototyping/Meshes/SM_Cube.SM_Cube"));
	UStaticMesh* Cube = MeshAsset.Object;


	// Set up Grid Mesh
	GridMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GridMesh"));
	SetRootComponent(GridMesh);
	GridMesh->SetStaticMesh(Cube);
	GridMesh->SetWorldScale3D(FVector3d(15.f, 15.f,.5f));

}


AGridTemplate::~AGridTemplate()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "Grid Destroyed");

	if (spawnVehicle->IsValidLowLevel()) spawnVehicle->Destroy(); 
	if (spawnVehicle1->IsValidLowLevel()) spawnVehicle1->Destroy(); 
	if (spawnTimePowerup->IsValidLowLevel()) spawnTimePowerup->Destroy();
}

// Called when the game starts or when spawned
void AGridTemplate::BeginPlay()
{
	Super::BeginPlay();

	// Set Reference to Player
	PlayerRef = Cast<ACarCentricCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

	// initialize new Grid Layout
	Layout.init(PlayerRef->GetCurrentDirection());

}

// Initialize new grid tempalte to spawn. 
// Vehicles and Powerups will spawn according to Layout
void AGridTemplate::Init()
{
	// Update location for new Grid/Powerup placements
	Layout.VehicleLocation_0 = FVector(this->GetActorLocation().X + 400.f, this->GetActorLocation().Y, 0.f);
	Layout.VehicleLocation_1 = FVector(this->GetActorLocation().X + 800.f, this->GetActorLocation().Y + 1500.f, 0.f);
	Layout.PowerupPlacement = FVector(this->GetActorLocation().X + Layout.PowerupPlacement.X, this->GetActorLocation().Y + Layout.PowerupPlacement.Y, 0.f);

	// Spawn the Powerups onto map
	spawnTimePowerup = Cast<ATimePowerup>(GetWorld()->SpawnActor<ATimePowerup>(Layout.PowerupPlacement, FRotator(0, 0, 0), SpawnInfo));
	// spawnTimePowerup->SetLifeSpan(20.0f);

	if (Layout.Direction == EGridDirection::FORWARD)
	{
		// spawnVehicle = Cast<AVehicle>(GetWorld()->SpawnActor<AVehicle>(vehicleClass, Layout.VehicleLocation_0, Layout.VehicleRotation_0, SpawnInfo));
		// spawnVehicle1 = Cast<AVehicle>(GetWorld()->SpawnActor<AVehicle>(vehicleClass, Layout.VehicleLocation_1, Layout.VehicleRotation_1, SpawnInfo));
		spawnVehicle = Cast<AVehicle>(GetWorld()->SpawnActor<AVehicle>(Layout.VehicleLocation_0, Layout.VehicleRotation_0, SpawnInfo));
		spawnVehicle1 = Cast<AVehicle>(GetWorld()->SpawnActor<AVehicle>(Layout.VehicleLocation_1, Layout.VehicleRotation_1, SpawnInfo));

	}
	else {

		Layout.VehicleRotation_0 = FRotator(0, 90, 0);
		Layout.VehicleRotation_1 = FRotator(0, 270, 0);
		Layout.VehicleLocation_0 = FVector(this->GetActorLocation().X + 1000.f, this->GetActorLocation().Y + 1000.f, 0.f);
		Layout.VehicleLocation_1 = FVector(this->GetActorLocation().X, this->GetActorLocation().Y + 200.f, 0.f);

		//spawnVehicle = Cast<AVehicle>(GetWorld()->SpawnActor<AVehicle>(vehicleClass, Layout.VehicleLocation_0, Layout.VehicleRotation_0, SpawnInfo));
		//spawnVehicle1 = Cast<AVehicle>(GetWorld()->SpawnActor<AVehicle>(vehicleClass, Layout.VehicleLocation_1, Layout.VehicleRotation_1, SpawnInfo));
		spawnVehicle = Cast<AVehicle>(GetWorld()->SpawnActor<AVehicle>(Layout.VehicleLocation_0, Layout.VehicleRotation_0, SpawnInfo));
		spawnVehicle1 = Cast<AVehicle>(GetWorld()->SpawnActor<AVehicle>(Layout.VehicleLocation_1, Layout.VehicleRotation_1, SpawnInfo));
	}
}

// Called every frame
void AGridTemplate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// ----------------------------------------------------------------------------------------------
// ---------------------------------Grid Layout Function Definitions ---------------------------- 
// ----------------------------------------------------------------------------------------------

// Initialize Grid Layout
FGridLayout::FGridLayout()
{

}

// Grab the next direcrion to spawn in grid
// If previouos direction was right, or left, prevent spawning a grid
// behind player
EGridDirection FGridLayout::GrabRandomDirection(uint8 direction)
{
	if (direction == 1)
	{
		int i = FMath::RandRange(0, 1);
		switch (i)
		{
		case 0:
			return EGridDirection::FORWARD;
		case 1:
			return EGridDirection::RIGHT;
		}

	}
	
	if (direction == 2)
	{
		int i = FMath::RandRange(0, 1);
		switch (i)
		{
		case 0:
			return EGridDirection::FORWARD;
		case 1:
			return EGridDirection::LEFT;
		}

	}

	int i = FMath::RandRange(0, 2);
	
	switch (i)
	{
	case 0 : 
		return EGridDirection::FORWARD;
	case 1 :
		return EGridDirection::RIGHT;
	case 2 :
		return EGridDirection::LEFT;
	default :
		return EGridDirection::FORWARD;
	}
}



