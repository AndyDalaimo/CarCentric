// Fill out your copyright notice in the Description page of Project Settings.


#include "Powerup.h"

// Sets default values
APowerup::APowerup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Type = SetRandomType();
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>TimeAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cone.Shape_Cone"));
	Cone = TimeAsset.Object;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>LargeTimeAsset(TEXT("/Game/LevelPrototyping/Meshes/SM_Cube.SM_Cube"));
	Cube = LargeTimeAsset.Object;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>HealthAsset(TEXT("/Game/LevelPrototyping/Meshes/SM_ChamferCube.SM_ChamferCube"));
	BlueCube = HealthAsset.Object;

	PowerupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PowerupMesh"));
	// SetRootComponent(PowerupMesh);
	PowerupMesh->SetRelativeScale3D(FVector(1.f, 1.f, 1.f));
	init();
	PowerupMesh->SetStaticMesh(BlueCube);
	PowerupMesh->SetGenerateOverlapEvents(true);
	PowerupMesh->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
}

// Initiate Powerup Type. Set appropriate mesh.
void APowerup::init()
{
	switch ((uint8)Type)
	{
	case (0):
		PowerupMesh->SetStaticMesh(Cube);
		return;
	case (1):
		PowerupMesh->SetStaticMesh(BlueCube);
		return;
	case (2):
		PowerupMesh->SetStaticMesh(Cone);
		return;
	}
}

// Called when the game starts or when spawned
void APowerup::BeginPlay()
{
	Super::BeginPlay();

	init();

	this->SetLifeSpan(4.f);
	
}

EPowerupType APowerup::SetRandomType()
{
	int i = FMath::RandRange(0, 2);
	switch (i)
	{
	case (0) :
		TimeIncrease = 3;
		HP = 0;
		Damage = 0;
		return EPowerupType::TIME;
	case (1) :
		TimeIncrease = 5;
		HP = 0;
		Damage = 5;
		return EPowerupType::LRGTIME;
	case (2) :
		TimeIncrease = 0;
		HP = 5;
		Damage = 0;
		return EPowerupType::HEALTH;
	}
	return EPowerupType();
}

// Called every frame
void APowerup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

