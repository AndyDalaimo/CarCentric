// Fill out your copyright notice in the Description page of Project Settings.




#include "Vehicle.h"


// Sets default values
AVehicle::AVehicle() : Damage(5), MovementTime(1.f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("/Game/StarterContent/Shapes/Shape_Pipe.Shape_Pipe"));
	UStaticMesh* Pipe = MeshAsset.Object;

	CarMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CarMesh"));
	SetRootComponent(CarMesh);
	CarMesh->SetRelativeScale3D(FVector(2.5f, 2.5f, 2.5f));
	CarMesh->SetStaticMesh(Pipe);
	CarMesh->SetGenerateOverlapEvents(false);

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	BoxCollider->AttachToComponent(CarMesh, FAttachmentTransformRules::KeepRelativeTransform);
	BoxCollider->SetBoxExtent(FVector(25, 52, 32), true);
	BoxCollider->SetHiddenInGame(false);
	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AVehicle::DamagePlayerOnCollision);

	VehiclePath = CreateDefaultSubobject<USplineComponent>(TEXT("VehiclePath"));
	VehiclePath->AttachToComponent(CarMesh, FAttachmentTransformRules::KeepRelativeTransform);
	VehiclePath->SetLocationAtSplinePoint(1, FVector(0, 700, 0), ESplineCoordinateSpace::Local, true);
	VehiclePath->Mobility = EComponentMobility::Movable;
	VehiclePath->SetVisibility(true);

	// Initialize the type of vehicle
	Type = TypeInit();

}

// Vehicle Destructor 
AVehicle::~AVehicle()
{
	UE_LOG(LogTemp, Warning, TEXT("Vehicle Destroyed"));
}

// Called when the game starts or when spawned
void AVehicle::BeginPlay()
{
	Super::BeginPlay();

	// -------------------------DEBUG DEBUG DEBUG-------------------------
	// Life span of Actor Currently Set
	this->SetLifeSpan(7.f);
	// -------------------------DEBUG DEBUG DEBUG-------------------------

	// Set Reference to Player
	PlayerRef = StaticCast<ACarCentricCharacter*>(GetWorld()->GetFirstPlayerController()->GetPawn());

	// Set Speed for Vehicle Movement
	Speed = SetSpeed(Type);

	UE_LOG(LogTemp, Warning, TEXT("New Vehicle Type and speed: %d and %f"), Type, Speed);

	// Set Damage for Vehicle Collision
	Damage = SetDamage(Type);

	// Set Current location at beginning of SplinePath
	currentLocation = VehiclePath->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::World);
	goalLocation = VehiclePath->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::World);

	// Start Movement timer
	MovementDelegate.BindUFunction(this, "MovementTimer");
	GetWorld()->GetTimerManager().SetTimer(MovementHandler, MovementDelegate, Speed, true);

}

// ---------------------------------------------------------------------------------------------
// -------------------------TODO -> Move Vehicle along this time line---------------------------
// ---------------------------------------------------------------------------------------------
// Move Vechicle Along road. At the end of timer, Destroy Actor
void AVehicle::MovementTimer(float movementSpeed)
{
	// Move Vehicle Along Spline
	currentLocation = FMath::VInterpConstantTo(currentLocation, goalLocation, GetWorld()->GetTimerManager().GetTimerElapsed(MovementHandler), 200.f);
	if(this->IsValidLowLevel()) this->SetActorLocation(currentLocation);	
}

// Collision Event when player is hit by Vehicle. 
// Will call damge event which will pass the damage amount (Dependant on vehicle type)
void AVehicle::DamagePlayerOnCollision(UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, const 
	FHitResult& SweepResult)
{
	if ((OtherActor == PlayerRef) && OtherComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player hit by Vehicle Type: %d Speed: %f"), Type, Speed);
		PlayerRef->PlayerDamaged(Damage);
	}
}



// Event Called when Player collides with vehicle
// Called in BP_Vehicle, Subtract damageAmount from Player HP
void AVehicle::DamagePlayer_Implementation(int32 damageAmount)
{
	UE_LOG(LogTemp, Warning, TEXT("Player took %d Damage"), damageAmount);
	
}

// Initialize new vehicle
EVehicleType AVehicle::TypeInit()
{
	int i = FMath::RandRange(0, 2);
	switch (i)
	{
	case 0:
		return EVehicleType::DEFAULT;
	case 1:
		return EVehicleType::COMPACT;
	case 2:
		return EVehicleType::TRUCK;
	}

	return EVehicleType::DEFAULT;
}

// Set Speed of Vehicle to be used in movement Timer
float AVehicle::SetSpeed(EVehicleType type)
{
	switch (type)
	{
		case EVehicleType::DEFAULT :
			return .08f;
		case EVehicleType::COMPACT :
			return .05f;
		case EVehicleType::TRUCK : 
			return .015f;
		default :
			return 0.0f;
	}
}

// Set damage for collision event with player
int32 AVehicle::SetDamage(EVehicleType type)
{
	switch (type)
	{
	case EVehicleType::DEFAULT:
		return 5;
	case EVehicleType::COMPACT:
		return 15;
	case EVehicleType::TRUCK:
		return 30;
	default:
		return 0;
	}
}

// Called every frame
/*void AVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}*/


