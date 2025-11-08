// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCubeActor.h"
#include "Components/TimelineComponent.h"

// Sets default values
AEnemyCubeActor::AEnemyCubeActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);

	MyStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	MyStaticMeshComponent->AttachToComponent(SceneComponent, FAttachmentTransformRules::KeepRelativeTransform);
	MyStaticMeshComponent->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));

	//Assign cube to static mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	MyStaticMeshComponent->SetStaticMesh(MeshAsset.Object);

	//Set the static mesh material
	static ConstructorHelpers::FObjectFinder<UMaterial> MeshMaterial(TEXT("Material'/Game/HandheldARBP/Materials/M_BackgroundFade.M_BackgroundFade'"));
	MyStaticMeshComponent->SetMaterial(0, MeshMaterial.Object);

	T = 0.f;
	isIncrementT = true;
}

// Called when the game starts or when spawned
void AEnemyCubeActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEnemyCubeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AEnemyCubeActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	initialLocation = GetActorLocation();
}

void AEnemyCubeActor::LemniscateMovement(float DeltaTime, FVector InitialLoc)
{
	initialLocation = InitialLoc;

	float scale = 2 / (3 - cos(2 * T));
	float x = scale * cos(T);
	float y = scale * sin(2 * T) / 2;

	//set new position along the infinity symbol and scale up movement by 100 so its movement is visible to player
	FVector CurrentPos = FVector(x * 10.f, y * 10.f, 0.0f);

	T += DeltaTime;

	FMatrix mat_init;

	mat_init = FMatrix::Identity;

	//put starting coorinates of cube
	mat_init.M[3][0] = initialLocation.X;
	mat_init.M[3][1] = initialLocation.Y;
	mat_init.M[3][2] = initialLocation.Z;

	FMatrix mat_moving = FMatrix::Identity;

	//put in the new calculated position of the cube along the infinity symbol
	mat_moving.M[3][0] = CurrentPos.X;
	mat_moving.M[3][1] = CurrentPos.Y;
	mat_moving.M[3][2] = CurrentPos.Z;

	//multiple matrices
	FMatrix mat_final = mat_moving * mat_init;

	//set new position of cube
	SetActorTransform(FTransform(mat_final));
}

void AEnemyCubeActor::MoveBetweenTwoImages(FVector ImgLoc1, FVector ImgLoc2, float DeltaTime)
{
	StartLoc = ImgLoc1;
	EndLoc = ImgLoc2;

	//check if T is more than 1 or less than 0
	if (T >= 1.f)
	{
		isIncrementT = false;
	}
	else if (T <= 0.f)
	{
		isIncrementT = true;
	}

	//if false the actor needs to move back to the first image
	if(!isIncrementT)
	{
		T -= 0.1f * 0.1;
	}	
	else if (isIncrementT) //if true the actor moves to the second image
	{
		T += 0.1f * 0.1;
	}	

	FVector currentPos = FMath::Lerp(StartLoc, EndLoc, T);
	SetActorLocation(currentPos);
}


