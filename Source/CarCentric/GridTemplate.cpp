// Fill out your copyright notice in the Description page of Project Settings.


#include "GridTemplate.h"

// Sets default values
AGridTemplate::AGridTemplate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("/Game/SimPoly_Town/Models/Road/SM_Road_02_B.SM_Road_02_B"));
	UStaticMesh* Road = MeshAsset.Object;


	// Set up Grid Mesh
	GridMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GridMesh"));
	SetRootComponent(GridMesh);
	GridMesh->SetStaticMesh(Road);
	// GridMesh->AddLocalOffset(FVector(500.f, 500.f, 0.f));
	GridMesh->SetWorldScale3D(FVector3d(1.2f, 1.2f,.5f));

	FallCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Fall Collider"));
	FallCollider->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	FallCollider->SetRelativeScale3D(FVector(35.f, 35.f, 2.f));
	FallCollider->AddLocalOffset(FVector(0, 0,-500));
	FallCollider->SetVisibility(true);

}

/*AGridTemplate::~AGridTemplate()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "Grid Destroyed");

	if (spawnVehicle->IsValidLowLevel()) spawnVehicle->Destroy(); 
	if (spawnVehicle1->IsValidLowLevel()) spawnVehicle1->Destroy(); 
	if (spawnTimePowerup->IsValidLowLevel()) spawnTimePowerup->Destroy();
}*/

// Called when the game starts or when spawned
void AGridTemplate::BeginPlay()
{
	Super::BeginPlay();

	// Set Reference to Player
	PlayerRef = Cast<ACarCentricCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

	FallCollider->OnComponentEndOverlap.AddUniqueDynamic(this, &AGridTemplate::KillPlayerOnCollision);

	// initialize new Grid Layout
	Layout.init(PlayerRef->GetCurrentDirection());

}

// Initialize new grid tempalte to spawn. 
// Vehicles and Powerups will spawn according to Layout
void AGridTemplate::Init()
{
	// Update location for new Grid/Powerup placements
	Layout.VehicleLocation_0 = FVector(this->GetActorLocation().X - FMath::RandRange(40.f, 450.f), this->GetActorLocation().Y - FMath::RandRange(40.f, 450.f), 0.f);
	Layout.VehicleLocation_1 = FVector(this->GetActorLocation().X + FMath::RandRange(40.f, 450.f), this->GetActorLocation().Y + FMath::RandRange(40.f, 450.f), 0.f);
	Layout.PowerupPlacement = FVector(this->GetActorLocation().X + Layout.PowerupPlacement.X, this->GetActorLocation().Y + Layout.PowerupPlacement.Y, 0.f);

	// Spawn the Powerups onto map
	SpawnPowerup();

	if (Layout.Direction == EGridDirection::FORWARD)
	{

		spawnVehicle = Cast<AVehicle>(GetWorld()->SpawnActor<AVehicle>(Layout.VehicleLocation_0, Layout.VehicleRotation_0, SpawnInfo));
		spawnVehicle1 = Cast<AVehicle>(GetWorld()->SpawnActor<AVehicle>(Layout.VehicleLocation_1, Layout.VehicleRotation_1, SpawnInfo));
		if (spawnVehicle->SpeedBoostActive) spawnVehicle->SpeedBoostRef->SetActorRotation(FRotator(0, 180, 0));
		if (spawnVehicle1->SpeedBoostActive) spawnVehicle1->SpeedBoostRef->SetActorRotation(FRotator(0, 0, 0));

	}
	else {

		Layout.VehicleRotation_0 = FRotator(0, 180, 0);
		Layout.VehicleRotation_1 = FRotator(0, 0, 0);
		Layout.VehicleLocation_0 = FVector(this->GetActorLocation().X + FMath::RandRange(40.f, 450.f), this->GetActorLocation().Y + FMath::RandRange(40.f, 450.f), 0.f);
		Layout.VehicleLocation_1 = FVector(this->GetActorLocation().X - FMath::RandRange(40.f, 450.f), this->GetActorLocation().Y - FMath::RandRange(40.f, 450.f), 0.f);


		spawnVehicle = Cast<AVehicle>(GetWorld()->SpawnActor<AVehicle>(Layout.VehicleLocation_0, Layout.VehicleRotation_0, SpawnInfo));
		spawnVehicle1 = Cast<AVehicle>(GetWorld()->SpawnActor<AVehicle>(Layout.VehicleLocation_1, Layout.VehicleRotation_1, SpawnInfo));
		if (spawnVehicle->SpeedBoostActive) spawnVehicle->SpeedBoostRef->SetActorRotation(FRotator(0, -90, 0));
		if (spawnVehicle1->SpeedBoostActive) spawnVehicle1->SpeedBoostRef->SetActorRotation(FRotator(0, 90, 0));

	}
}

// Choose a powerup to spawn
// Currently Spawning random powerup
void AGridTemplate::SpawnPowerup()
{
	int i = FMath::RandRange(0, 2);

	switch (i)
	{
	case (0) :
		spawnTimePowerup = Cast<ATimePowerup>(GetWorld()->SpawnActor<ATimePowerup>(Layout.PowerupPlacement, FRotator(0, 0, 0), SpawnInfo));
		return;
	case (1) :
		spawnLargeTimePowerup = Cast<ALargeTimePowerup>(GetWorld()->SpawnActor<ALargeTimePowerup>(Layout.PowerupPlacement, FRotator(0, 0, 0), SpawnInfo));
		return;
	case (2) :
		if (PlayerRef->HP < 40.f) spawnHealthPowerup = Cast<AHealthPowerup>(GetWorld()->SpawnActor<AHealthPowerup>(Layout.PowerupPlacement, FRotator(0, 0, 0), SpawnInfo));
		else spawnTimePowerup = Cast<ATimePowerup>(GetWorld()->SpawnActor<ATimePowerup>(Layout.PowerupPlacement, FRotator(0, 0, 0), SpawnInfo));
		return;
	}
}

// Game Over if Player falls off of map
void AGridTemplate::KillPlayerOnCollision(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if ((OtherActor == PlayerRef) && OtherComp) {
		PlayerRef->GameInstanceRef->ShowGameOverUIWidget();
		PlayerRef->Destroy();
	}
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("PLayer Falling"));
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



