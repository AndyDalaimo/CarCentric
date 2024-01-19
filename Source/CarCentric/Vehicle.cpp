// Fill out your copyright notice in the Description page of Project Settings.




#include "Vehicle.h"


// Sets default values
AVehicle::AVehicle() : Damage(5), MovementTime(1.f), SpeedBoostActive(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SmallCarMeshAsset(TEXT("/Game/SimPoly_Town/Models/Car/SK_Car_01.SK_Car_01"));
	SmallCar = SmallCarMeshAsset.Object;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>MediumCarMeshAsset(TEXT("/Game/SimPoly_Town/Models/Car/SK_Car_02.SK_Car_02"));
	MediumCar = MediumCarMeshAsset.Object;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>LargeCarMeshAsset(TEXT("/Game/SimPoly_Town/Models/Car/SK_Car_04.SK_Car_04"));
	LargeCar = LargeCarMeshAsset.Object;

	CarMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CarMesh"));
	SetRootComponent(CarMesh);
	// CarMesh->SetSkeletalMesh(SmallCar);
	CarMesh->SetRelativeScale3D(FVector(.8f, .8f, .8f));
	CarMesh->SetGenerateOverlapEvents(true);
	CarMesh->SetCollisionProfileName(TEXT("IgnoreOnlyPawn"));

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	BoxCollider->AttachToComponent(CarMesh, FAttachmentTransformRules::KeepRelativeTransform);
	BoxCollider->SetBoxExtent(FVector(160, 90, 150), true);
	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AVehicle::DamagePlayerOnCollision);

	VehiclePath = CreateDefaultSubobject<USplineComponent>(TEXT("VehiclePath"));
	VehiclePath->AttachToComponent(CarMesh, FAttachmentTransformRules::KeepRelativeTransform);
	VehiclePath->SetRelativeRotation(FRotator(0, -90, 0));
	VehiclePath->SetLocationAtSplinePoint(1, FVector(0, 2000, 0), ESplineCoordinateSpace::Local, true);
	VehiclePath->Mobility = EComponentMobility::Movable;

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

	// Initialize the type of vehicle
	Type = TypeInit();

	GameInstanceRef = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());

	// Set Reference to Player
	PlayerRef = StaticCast<ACarCentricCharacter*>(GetWorld()->GetFirstPlayerController()->GetPawn());

	switch (Type) {
		case (EVehicleType::DEFAULT) :
			CarMesh->SetSkeletalMesh(SmallCar);
			SpeedBoostActive = CreateSpeedBoost();
			break;
		case (EVehicleType::COMPACT) :
			CarMesh->SetSkeletalMesh(MediumCar);
			break;
		case (EVehicleType::TRUCK) :
			CarMesh->SetSkeletalMesh(LargeCar);
			BoxCollider->SetBoxExtent(FVector(200, 90, 300), true);
			break;
		default :
			break;
	}

	/*if (Type == EVehicleType::DEFAULT)
	{
		CarMesh->SetSkeletalMesh(SmallCar);
		SpeedBoostActive = CreateSpeedBoost();
	}
	else if (Type == EVehicleType::COMPACT) CarMesh->SetSkeletalMesh(MediumCar);
	else
	{
		CarMesh->SetSkeletalMesh(LargeCar);
		BoxCollider->SetBoxExtent(FVector(200, 90, 300), true);

	}*/

	// -------------------------DEBUG DEBUG DEBUG-------------------------
	// Life span of Actor Currently Set
	this->SetLifeSpan(15.f);
	// -------------------------DEBUG DEBUG DEBUG-------------------------

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
	GetWorld()->GetTimerManager().SetTimer(MovementHandler, MovementDelegate, Speed, true, FMath::RandRange(2.f, 4.5f));

}


// Move Vechicle Along road. At the end of timer, Destroy Actor
void AVehicle::MovementTimer(float movementSpeed)
{
	// Move Vehicle Along Spline
	currentLocation = FMath::VInterpConstantTo(currentLocation, goalLocation, GetWorld()->GetTimerManager().GetTimerElapsed(MovementHandler), 200.f);
	if(this->IsValidLowLevel()) this->SetActorLocation(currentLocation);
	if (this->GetActorLocation() == goalLocation) this->Destroy();
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
		PlayerRef->PlayerHitByVehicle();
		PlayerRef->PlayerDamaged(Damage);
		MovementHandler.Invalidate();
	}
}



// Event Called when Player collides with vehicle
// Called in BP_Vehicle, Subtract damageAmount from Player HP
void AVehicle::DamagePlayer_Implementation(float damageAmount)
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
	default: break;
	}

	return EVehicleType::DEFAULT;
}

// Set Speed of Vehicle to be used in movement Timer
float AVehicle::SetSpeed(EVehicleType type)
{
	switch (type)
	{
		case EVehicleType::DEFAULT :
			return .03f;
		case EVehicleType::COMPACT :
			return .02f;
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

// Only Spawn speed boost powerup if Game Timer is above a certain threshold
bool AVehicle::CreateSpeedBoost()
{
	// if the total time is above 10 and current GlobalTimeDilation is not at max, spawn a powerup for player, else return fale
	if (GameInstanceRef->totalTime > 10 && PlayerRef->GlobalTime < 2.f && FMath::RandRange(0, 4) < 1)
	{
		SpeedBoostRef = GetWorld()->SpawnActor<ASpeedBoostPowerup>(this->GetActorLocation(), this->GetActorRotation(), SpawnInfo);
		SpeedBoostRef->AttachToComponent(CarMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("BirdSocket"));
		// SpeedBoostRef->SetActorRotation(FRotator(0, 0, 0));
		return true;
	}
		
	return false;

}

// Called every frame
/*void AVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}*/


