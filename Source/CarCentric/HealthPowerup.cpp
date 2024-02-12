// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPowerup.h"

// Sets default values
AHealthPowerup::AHealthPowerup() : HP(5)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("/Game/Assets/Heart.Heart"));
	UStaticMesh* Heart = MeshAsset.Object;

	HeartMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeartMesh"));
	SetRootComponent(HeartMesh);
	HeartMesh->SetRelativeScale3D(FVector(40.f, 40.f, 40.f));
	HeartMesh->SetRelativeRotation(FRotator(0, 0, 90));
	HeartMesh->SetStaticMesh(Heart);
	HeartMesh->bApplyImpulseOnDamage = false;
	// HeartMesh->SetGenerateOverlapEvents(true);
	// HeartMesh->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	BoxCollider->AttachToComponent(HeartMesh, FAttachmentTransformRules::KeepRelativeTransform);
	BoxCollider->SetBoxExtent(FVector(1.5f, 1.5f, 1.5f), true);
	BoxCollider->bApplyImpulseOnDamage = false;
	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AHealthPowerup::CollectHealthPowerup);

}

// Called when the game starts or when spawned
void AHealthPowerup::BeginPlay()
{
	Super::BeginPlay();
	
	this->SetLifeSpan(10.f);

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

