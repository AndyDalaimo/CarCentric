// Fill out your copyright notice in the Description page of Project Settings.


#include "LargeTimePowerup.h"

// Sets default values
ALargeTimePowerup::ALargeTimePowerup() : timeIncrease(5), damage(5)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("/Game/Assets/DoctorsNote.DoctorsNote"));
	UStaticMesh* Cube = MeshAsset.Object;

	PowerupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ClockMesh"));
	PowerupMesh->SetupAttachment(RootComponent);
	PowerupMesh->SetStaticMesh(Cube);
	PowerupMesh->SetRelativeScale3D(FVector3d(40.f, 40.f, 40.f));
	PowerupMesh->SetRelativeRotation(FRotator(0, 90, 90));
	PowerupMesh->SetCollisionProfileName(TEXT("IgnoreAll"));
	PowerupMesh->bApplyImpulseOnDamage = false;

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	BoxCollider->AttachToComponent(PowerupMesh, FAttachmentTransformRules::KeepRelativeTransform);
	BoxCollider->SetBoxExtent(FVector(1.5, 1.5, 1.5), true);
	BoxCollider->AddRelativeLocation(FVector(0, -2, 0));
	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &ALargeTimePowerup::CollectPowerup);
	BoxCollider->bApplyImpulseOnDamage = false;

}

// Called when the game starts or when spawned
void ALargeTimePowerup::BeginPlay()
{
	Super::BeginPlay();

	// Life Span of Actor in World
	this->SetLifeSpan(10.f);

	// Set reference to Player
	PlayerRef = Cast<ACarCentricCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

	// Set Reference to Game Instance
	GameInstanceRef = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());
}

// Event Called when Player picks up time powerup. Will call event to update player timer and destroy object
void ALargeTimePowerup::CollectPowerup(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if ((OtherActor == PlayerRef) && (OtherActor != this) && OtherComp)
	{
		// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Time Powerup Collected"));
		GameInstanceRef->TimePowerupCollect(timeIncrease);
		PlayerRef->PlayerDamaged(damage);
		// UpdateGameInstanceTimer();
		Destroy();
	}
}

// Called every frame
void ALargeTimePowerup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

