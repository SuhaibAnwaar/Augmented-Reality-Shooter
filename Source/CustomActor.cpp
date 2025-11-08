// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomActor.h"
#include "Kismet/GameplayStatics.h"
#include "CustomARPawn.h"
#include "Camera/CameraComponent.h"

// Sets default values
ACustomActor::ACustomActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);

	MyStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	MyStaticMeshComponent->AttachToComponent(SceneComponent, FAttachmentTransformRules::KeepRelativeTransform);
	MyStaticMeshComponent->SetRelativeScale3D(FVector(0.05f));

	//Assign cube to static mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	MyStaticMeshComponent->SetStaticMesh(MeshAsset.Object);

	T = 0.f;
	LerpMultiplier = 0.0f;
}

// Called when the game starts or when spawned
void ACustomActor::BeginPlay()
{
	Super::BeginPlay();

	PlayerRef = Cast<ACustomARPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	if (PlayerRef)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::White, "PlayerRef is Valid");
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::White, "PlayerRef is Not Valid");
	}
}

// Called every frame
void ACustomActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (LerpMultiplier < 1.1f)
	{
		LerpMultiplier += 0.1 * 0.1;
	}
	
	//Constantly feed in the Camera's current position
	MoveTowardsPlayer(PlayerRef->CameraComponent->GetComponentLocation());
}

void ACustomActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	initialLocation = GetActorLocation();
}

void ACustomActor::LemniscateMovement(float DeltaTime, FVector InitialLoc)
{
	initialLocation = InitialLoc;

	float scale = 2 / (3 - cos(2 * T));
	float x = scale * cos(T);
	float y = scale * sin(2 * T) / 2;

	//position of cube relative to the origin
	//float meshOffset = 150.f;

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

void ACustomActor::MoveTowardsPlayer(FVector PlayerStartingPos)
{
	FVector CurrentPos = FMath::Lerp(initialLocation, PlayerStartingPos, LerpMultiplier);

	SetActorLocation(CurrentPos);
}

void ACustomActor::OnOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACustomARPawn* LocalPawn = Cast<ACustomARPawn>(OtherActor);

}

