// Fill out your copyright notice in the Description page of Project Settings.

#include "SpeedBoostPowerup.h"

// Sets default values
ASpeedBoostPowerup::ASpeedBoostPowerup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>BirdAsset(TEXT("/Game/SimPoly_Town/Models/Bird/SK_Bird.SK_Bird"));
	USkeletalMesh* Bird = BirdAsset.Object;

	static ConstructorHelpers::FObjectFinder<UAnimationAsset>AnimAsset(TEXT("/Game/SimPoly_Town/Models/Bird/A_Bird.A_Bird"));
	// static ConstructorHelpers::FClassFinder
	UAnimationAsset* BirdAnim = AnimAsset.Object;

	PowerupMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Bird"));
	SetRootComponent(PowerupMesh);
	PowerupMesh->SetSkeletalMesh(Bird);
	PowerupMesh->OverrideAnimationData(BirdAnim, true, true);

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	BoxCollider->AttachToComponent(PowerupMesh, FAttachmentTransformRules::KeepRelativeTransform);
	BoxCollider->bHiddenInGame = false;
	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &ASpeedBoostPowerup::PowerupPlayerOnCollision);

}

// Called when the game starts or when spawned
void ASpeedBoostPowerup::BeginPlay()
{
	Super::BeginPlay();

	this->SetLifeSpan(10.f);

	// Reference to Game Instance
	GameInstanceRef = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());

	// Reference to Player
	PlayerRef = StaticCast<ACarCentricCharacter*>(GetWorld()->GetFirstPlayerController()->GetPawn());

	// PowerupMesh->PlayAnimation();
}

// Will increase the speed of game (Global Time Dilation) when Player Interacts with this powerup
void ASpeedBoostPowerup::PowerupPlayerOnCollision(UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex,
	bool bFromSweep, 
	const FHitResult& SweepResult)
{
	if ((OtherActor == PlayerRef) && OtherComp)
	{
		// UE_LOG(LogTemp, Warning, TEXT("Player hit by Vehicle Type: %d Speed: %f"), Type, Speed);
		PlayerRef->SpeedBoost(1.f);
		PowerupMesh->SetHiddenInGame(true);
	}
}

// Called every frame
/*void ASpeedBoostPowerup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}*/

