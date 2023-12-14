// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPowerup.h"

// Sets default values
AHealthPowerup::AHealthPowerup() : HP(5)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cone.Shape_Cone"));
	UStaticMesh* Cone = MeshAsset.Object;

	HeartMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeartMesh"));
	SetRootComponent(HeartMesh);
	HeartMesh->SetRelativeScale3D(FVector(1.f, 1.f, 1.f));
	HeartMesh->SetStaticMesh(Cone);
	// HeartMesh->SetGenerateOverlapEvents(true);
	// HeartMesh->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	BoxCollider->AttachToComponent(HeartMesh, FAttachmentTransformRules::KeepRelativeTransform);
	BoxCollider->SetBoxExtent(FVector(60, 60, 60), true);
	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AHealthPowerup::CollectHealthPowerup);

}

// Called when the game starts or when spawned
void AHealthPowerup::BeginPlay()
{
	Super::BeginPlay();
	
	this->SetLifeSpan(4.f);

	// Set reference to Player
	PlayerRef = Cast<ACarCentricCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
}

// Collision event to trigger when player interacts with health powerup
void AHealthPowerup::CollectHealthPowerup(UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult& SweepResult)
{
	if ((OtherActor == PlayerRef)  && (OtherActor != this) && OtherComp)
	{
		PlayerRef->PlayerHealed(HP);
		Destroy();
	}

}

// Called every frame
void AHealthPowerup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

