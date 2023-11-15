// Fill out your copyright notice in the Description page of Project Settings.


#include "GridTemplate.h"

// Sets default values
AGridTemplate::AGridTemplate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("/Game/LevelPrototyping/Meshes/SM_Cube.SM_Cube"));
	UStaticMesh* Cube = MeshAsset.Object;
	

	GridMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GridMesh"));
	SetRootComponent(GridMesh);
	// GridMesh->SetupAttachment(RootComponent);
	GridMesh->SetStaticMesh(Cube);
	GridMesh->SetWorldScale3D(FVector3d(20.f, 20.f,.5f));

	VehiclePath = CreateDefaultSubobject<USplineComponent>(TEXT("VehiclePath"));
	VehiclePath->AttachToComponent(GridMesh, FAttachmentTransformRules::KeepRelativeTransform);
	VehiclePath->Mobility = EComponentMobility::Movable;
	VehiclePath->SetVisibility(true);
	VehiclePath->SetRelativeLocationAndRotation(FVector(25.f, 0.f, 0.f), FRotator(0, 90, 0));

	VehiclePath1 = CreateDefaultSubobject<USplineComponent>(TEXT("VehiclePath1"));
	VehiclePath1->AttachToComponent(GridMesh, FAttachmentTransformRules::KeepRelativeTransform);
	VehiclePath1->Mobility = EComponentMobility::Movable;
	VehiclePath1->SetVisibility(true);
	VehiclePath1->SetRelativeLocationAndRotation(FVector(75.f, 0.f, 0.f), FRotator(0, 90, 0));


}


/*AGridTemplate::~AGridTemplate()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "Grid Destroyed");
}*/

// Called when the game starts or when spawned
void AGridTemplate::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGridTemplate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

