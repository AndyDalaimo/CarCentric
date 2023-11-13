// Fill out your copyright notice in the Description page of Project Settings.


#include "GridTemplate.h"

// Sets default values
AGridTemplate::AGridTemplate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("/Game/LevelPrototyping/Meshes/SM_Cube.SM_Cube"));
	UStaticMesh* Asset = MeshAsset.Object;
	

	GridMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GridMesh"));
	GridMesh->SetupAttachment(RootComponent);
	GridMesh->SetStaticMesh(Asset);
	GridMesh->SetWorldScale3D(FVector3d(20.f, 20.f,.5f));

	
}

AGridTemplate::~AGridTemplate()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "Grid Destroyed");
}

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

