// Fill out your copyright notice in the Description page of Project Settings.


#include "GridTemplate.h"

// Sets default values
AGridTemplate::AGridTemplate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<AVehicle> VehicleFinder(TEXT("/Game/WorldItems/BP_Vehicle"));
	vehicleClass = VehicleFinder.Class;

	if (VehicleFinder.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("BP FOUND"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("BP NOT FOUND"));
		vehicleClass = AVehicle::StaticClass();
	}


	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("/Game/LevelPrototyping/Meshes/SM_Cube.SM_Cube"));
	UStaticMesh* Cube = MeshAsset.Object;

	Layout.init();


	GridMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GridMesh"));
	SetRootComponent(GridMesh);
	// GridMesh->SetupAttachment(RootComponent);
	GridMesh->SetStaticMesh(Cube);
	GridMesh->SetWorldScale3D(FVector3d(20.f, 20.f,.5f));

}


/*AGridTemplate::~AGridTemplate()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "Grid Destroyed");
}*/

// Called when the game starts or when spawned
void AGridTemplate::BeginPlay()
{
	Super::BeginPlay();
	// NOT THE RIGHT LOCATION
	Layout.VehicleLocation_0 = FVector(this->GetActorLocation().X + 600.f, this->GetActorLocation().Y, 0.f);
	Layout.VehicleLocation_1 = FVector(this->GetActorLocation().X + 1200.f, this->GetActorLocation().Y + 2000.f, 0.f);
	

	if (Layout.Direction == EGridDirection::FORWARD)
	{
		spawnVehicle = Cast<AVehicle>(GetWorld()->SpawnActor<AVehicle>(vehicleClass, Layout.VehicleLocation_0, Layout.VehicleRotation_0, SpawnInfo));
		spawnVehicle1 = Cast<AVehicle>(GetWorld()->SpawnActor<AVehicle>(vehicleClass, Layout.VehicleLocation_1, Layout.VehicleRotation_1, SpawnInfo));
		
	}
	else {

		Layout.VehicleLocation_0 = FVector(this->GetActorLocation().X, this->GetActorLocation().Y + 600.f, 0.f);
		Layout.VehicleLocation_1 = FVector(this->GetActorLocation().X, this->GetActorLocation().Y + 1200.f, 0.f);
		Layout.VehicleRotation_0 = FRotator(0,90,0);
		Layout.VehicleRotation_1 = FRotator(0,270,0);

		spawnVehicle = Cast<AVehicle>(GetWorld()->SpawnActor<AVehicle>(vehicleClass, Layout.VehicleLocation_0, Layout.VehicleRotation_0, SpawnInfo));
		spawnVehicle1 = Cast<AVehicle>(GetWorld()->SpawnActor<AVehicle>(vehicleClass, Layout.VehicleLocation_1, Layout.VehicleRotation_1, SpawnInfo));
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

EGridDirection FGridLayout::GrabRandomDirection()
{
	int i = rand() % 5;
	
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



