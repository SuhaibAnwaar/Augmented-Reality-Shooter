// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletSphereActor.h"

// Sets default values
ABulletSphereActor::ABulletSphereActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);

	MyStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	MyStaticMeshComponent->AttachToComponent(SceneComponent, FAttachmentTransformRules::KeepRelativeTransform);
	MyStaticMeshComponent->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));

	//Assign cube to static mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	MyStaticMeshComponent->SetStaticMesh(MeshAsset.Object);

	//Set the static mesh material
	static ConstructorHelpers::FObjectFinder<UMaterial> MeshMaterial(TEXT("Material'/Game/HandheldARBP/Materials/M_BackgroundFade.M_BackgroundFade'"));
	MyStaticMeshComponent->SetMaterial(0, MeshMaterial.Object);

	T = 0.f;
}

// Called when the game starts or when spawned
void ABulletSphereActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABulletSphereActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABulletSphereActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ABulletSphereActor::LemniscateMovement(float DeltaTime, FVector InitialLoc)
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

	//put starting coorinates of sphere
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

	//set new position of sphere
	SetActorTransform(FTransform(mat_final));
}


