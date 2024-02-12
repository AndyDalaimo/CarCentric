// Fill out your copyright notice in the Description page of Project Settings.


#include "TimePowerup.h"
#include "Engine/World.h"

// Sets default values
ATimePowerup::ATimePowerup() : timeIncrease(3)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("/Game/Assets/Stopwatch.Stopwatch"));
	UStaticMesh* Cube = MeshAsset.Object;

	ClockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ClockMesh"));
	ClockMesh->SetupAttachment(RootComponent);
	ClockMesh->SetStaticMesh(Cube);
	ClockMesh->bApplyImpulseOnDamage = false;
	ClockMesh->SetRelativeScale3D(FVector3d(40.f, 40.f, 40.f));
	ClockMesh->SetRelativeRotation(FRotator(0, 90, 90));

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	BoxCollider->AttachToComponent(ClockMesh, FAttachmentTransformRules::KeepRelativeTransform);
	// BoxCollider->SetWorldScale3D(FVector3d(2.f, 2.f, 2.f));
	BoxCollider->SetBoxExtent(FVector(1.5f, 1.5f, 1.5f), true);
	BoxCollider->AddRelativeLocation(FVector(0, -2, 0));
	BoxCollider->bApplyImpulseOnDamage = false;
	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &ATimePowerup::CollectPowerup);
}

// Called when the game starts or when spawned
void ATimePowerup::BeginPlay()
{
	Super::BeginPlay();

	// Life Span of Actor in World
	this->SetLifeSpan(10.f);

	// Set reference to Player
	PlayerRef = Cast<ACarCentricCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

	// Set Reference to Game Instance
	GameInstanceRef = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());
}

// Set new Powerup Type
void ATimePowerup::Init()
{
}

// Called every frame
/*void ATimePowerup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}*/

// Event Called when Player picks up time powerup. Will call event to update player timer and destroy object
void ATimePowerup::CollectPowerup(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if ((OtherActor == PlayerRef) && (OtherActor != this) && OtherComp)
	{
		// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Large Time Powerup Collected"));
		GameInstanceRef->TimePowerupCollect(timeIncrease);
		// UpdateGameInstanceTimer();
		Destroy();
	}
}

// Blueprint Event To update in game timer
void ATimePowerup::UpdateGameInstanceTimer_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Update Game Timer"));
}