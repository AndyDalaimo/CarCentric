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
	CarMesh->SetupAttachment(RootComponent);
	CarMesh->SetRelativeScale3D(FVector(2.5f, 2.5f, 2.5f));
	CarMesh->SetStaticMesh(Pipe);
	CarMesh->SetGenerateOverlapEvents(false);

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	BoxCollider->AttachToComponent(CarMesh, FAttachmentTransformRules::KeepRelativeTransform);
	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AVehicle::DamagePlayerOnCollision);

	VehiclePath = CreateDefaultSubobject<USplineComponent>(TEXT("VehiclePath"));
	VehiclePath->AttachToComponent(CarMesh, FAttachmentTransformRules::KeepRelativeTransform);
	VehiclePath->SetLocationAtSplinePoint(1, FVector(0, 800, 0), ESplineCoordinateSpace::Local, true);
	VehiclePath->Mobility = EComponentMobility::Movable;
	VehiclePath->SetVisibility(true);

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

	// this->SetLifeSpan(7.f);

	// Set Reference to Player
	PlayerRef = StaticCast<ACarCentricCharacter*>(GetWorld()->GetFirstPlayerController()->GetPawn());
	
	// Set Speed for Vehicle Movement
	Speed = SetSpeed(Type);

	// Start Movement timer
	MovementDelegate.BindUFunction(this, "MovementTimer");
	GetWorld()->GetTimerManager().SetTimer(MovementHandler, MovementDelegate, Speed/10, true);

}

// ---------------------------------------------------------------------------------------------
// -------------------------TODO -> Move Vehicle along this time line---------------------------
// ---------------------------------------------------------------------------------------------
// Move Vechicle Along road. At the end of timer, Destroy Actor
void AVehicle::MovementTimer(float movementSpeed)
{
	if(this->IsValidLowLevel()) this->SetActorLocation((this->GetActorLocation() + FVector(0.f, 100.f, 0.f)), true);	
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
	if ((OtherActor == PlayerRef) && (OtherActor != this) && OtherComp)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Player Hit By Vehicle"));
		DamagePlayer(Damage);
		GetWorld()->GetTimerManager().ClearTimer(MovementHandler);

	}
}

// Event Called when Player collides with vehicle
// Called in BP_Vehicle, Subtract damageAmount from Player HP
void AVehicle::DamagePlayer_Implementation(int32 damageAmount)
{
	UE_LOG(LogTemp, Warning, TEXT("Player took %d Damage"), damageAmount);
}


// Set Speed of Vehicle to be used in movement Timer
float AVehicle::SetSpeed(EVehicleType type)
{
	switch (type)
	{
		case EVehicleType::DEFAULT :
			return 1.0f;
		case EVehicleType::COMPACT :
			return 5.0f;
		case EVehicleType::TRUCK : 
			return 8.0f;
		default :
			return 0.0f;
	}
}




// Called every frame
/*void AVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}*/


