// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomARPawn.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "ARBlueprintLibrary.h"
#include "Camera/CameraComponent.h"
#include "TimerManager.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "CustomActor.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnemyCubeActor.h"
#include "BulletSphereActor.h"
#include "PlaneActor.h"

// Sets default values
ACustomARPawn::ACustomARPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SceneComponent);
	CameraComponent->SetRelativeLocation(FVector(-100.0f, 0.0f, 150.f));
	CameraComponent->SetRelativeRotation(FRotator(-25.f, 0.0f, 0.f));
	
	//load the session config
	static ConstructorHelpers::FObjectFinder<UARSessionConfig> CofigAsset(TEXT("ARSessionConfig'/Game/ARImages/D_ARSessionConfig.D_ARSessionConfig'"));

	if (CofigAsset.Succeeded())
	{
		globalConfig = CofigAsset.Object;
	}

	//Get red material
	static ConstructorHelpers::FObjectFinder<UMaterial> RedMeshMaterial(TEXT("Material'/Game/HandheldARBP/Materials/M_Proximity_Red.M_Proximity_Red'"));

	//get white material
	static ConstructorHelpers::FObjectFinder<UMaterial> WhiteMeshMaterial(TEXT("Material'/Game/HandheldARBP/Materials/M_BackgroundFade.M_BackgroundFade'"));

	if (RedMeshMaterial.Succeeded())
	{
		//assign material to global variables
		RedMaterial = RedMeshMaterial.Object;
	}

	if (WhiteMeshMaterial.Succeeded())
	{
		//assign material to global variables
		WhiteMaterial = WhiteMeshMaterial.Object;
	}

	//initialise values
	cameraNotifyLoopTime = 4.0f;

	ObjectYPos = 0.0f;

	bSpawnCube = false;
	isCubeClicked = false;
	isSphereClicked = false;

	bGoghFound = false;
	bEarthFound = false;
	bMovingCubeFound = false;

	SpawnLocGogh = FVector(0.0f);

	Scanning = 0.f;
	ScreenPos = FVector2D(0.f);
	bScreenTouched = false;

	bPlaneActorFound = false;
}

// Called when the game starts or when spawned
void ACustomARPawn::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ACustomARPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//get center of screen
	GEngine->GameViewport->GetViewportSize(ScreenPos);
	ScreenPos /= 2.f;
}

// Called to bind functionality to input
void ACustomARPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindTouch(IE_Pressed, this, &ACustomARPawn::OnScreenTouch);
	PlayerInputComponent->BindTouch(IE_Released, this, &ACustomARPawn::OnScreenRelease);
}

//Set a material to an object based of its proximity to the player
void ACustomARPawn::SetMeshMaterial(bool isCube)
{
	TArray<AActor*> FoundActors;
	
	//the isCube variable determines if we are checking for cubes or sphere
	if (isCube)
	{
		//cube within 100 units
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyCubeActor::StaticClass(), FoundActors);
	}
	else
	{
		//Sphere within 100 units
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABulletSphereActor::StaticClass(), FoundActors);
	}

	for (int i = 0; i < FoundActors.Num(); i++)
	{
		if (GetDistanceTo(FoundActors[i]) <= 100.f)
		{
			if (isCube)
			{
				AEnemyCubeActor* EnemyCubeActor = Cast<AEnemyCubeActor>(FoundActors[i]);
				if (EnemyCubeActor)
				{
					EnemyCubeActor->MyStaticMeshComponent->SetMaterial(0, RedMaterial);
				}
			}
			else
			{
				ABulletSphereActor* BulletSphereActor = Cast<ABulletSphereActor>(FoundActors[i]);
				if (BulletSphereActor)
				{
					BulletSphereActor->MyStaticMeshComponent->SetMaterial(0, RedMaterial);
				}
			}
		}
		else //out of range
		{
			if (isCube)
			{
				AEnemyCubeActor* EnemyCubeActor = Cast<AEnemyCubeActor>(FoundActors[i]);
				if (EnemyCubeActor)
				{
					EnemyCubeActor->MyStaticMeshComponent->SetMaterial(0, WhiteMaterial);
				}
			}
			else
			{
				ABulletSphereActor* BulletSphereActor = Cast<ABulletSphereActor>(FoundActors[i]);
				if (BulletSphereActor)
				{
					BulletSphereActor->MyStaticMeshComponent->SetMaterial(0, WhiteMaterial);
				}
			}
		}
	}
}

//Print current camera orientation as debug message.
void ACustomARPawn::DisplayCameraInfo()
{
	//camera location and rotation
	FVector camLoc;
	FRotator camRot;

	//get camera details
	UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraViewPoint(camLoc, camRot);

	//convert rotation into a vector as camera orientation
	FVector camOri = camRot.Vector();

	//Print to screen
	//UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Camera orientation: (%f, %f, %f)"), camOri.X, camOri.Y, camOri.Z), true, true, FLinearColor(0, 0.66, 1, 1), 5);
}

void ACustomARPawn::SpawnObject(FVector InImageLoc)
{
	//spawn object either in the origin, left of origin or right of origin
	/*if (ObjectYPos == 0.0f)
	{
		ObjectYPos = 100.f;
	}
	else if (ObjectYPos == 100.f)
	{
		ObjectYPos = -100.f;
	}
	else
	{
		ObjectYPos = 0.f;
	}*/

	//set position and rotation of spawning object
	FActorSpawnParameters SpawnInfo;
	FRotator myRot(0, 0, 0);
	//FVector myLoc(300, ObjectYPos, 0);
	FVector myLoc = InImageLoc;

	//alternate between spawning a cube and a sphere
	if (!bSpawnCube)
	{
		EarthSphere = GetWorld()->SpawnActor<ABulletSphereActor>(myLoc, myRot, SpawnInfo);		
	}
	else
	{
		GoghCube = GetWorld()->SpawnActor<ACustomActor>(myLoc, myRot, SpawnInfo);
	}
}

void ACustomARPawn::OnScreenTouch(const ETouchIndex::Type fingerIndex, const FVector screenPos)
{
	FHitResult hitTestResult;
	
	//Perform a hitTest, get the return values as hitTestResult
	if (!WorldHitTest(FVector2D(screenPos), hitTestResult))
	{
		//record position of where the cube was clicked
		if (isCubeClicked && !isSphereClicked)
		{
			initalTouchLoc = screenPos;
		}
		else if (!isCubeClicked && isSphereClicked)//record the position of where the sphere was clicked
		{
			initalTouchLoc = screenPos;
		}

		return;
	}

	//get object of actor hit
	UClass* hitActorClass = UGameplayStatics::GetObjectClass(hitTestResult.GetActor());

	//if target hit is a cube from the AEnemyCubeActor class
	if (UKismetMathLibrary::ClassIsChildOf(hitActorClass, AEnemyCubeActor::StaticClass()))
	{
		isCubeClicked = true;
		isSphereClicked = false;

		//initialise global variable reference to class
		enemyCubeActorRef = Cast<AEnemyCubeActor>(hitTestResult.GetActor());
	}

	//if target hit is a cube from the AEnemyCubeActor class
	if (UKismetMathLibrary::ClassIsChildOf(hitActorClass, ACustomActor::StaticClass()))
	{
		ACustomActor* CA = Cast<ACustomActor>(hitTestResult.GetActor());

		if (CA)
		{
			CA->Destroy();
		}
	}

	//if target hit is a sphere
	if (UKismetMathLibrary::ClassIsChildOf(hitActorClass, ABulletSphereActor::StaticClass()))
	{
		isSphereClicked = true;
		isCubeClicked = false;

		//initialise global variable reference to class
		bulletSphereActorRef = Cast<ABulletSphereActor>(hitTestResult.GetActor());
	}

	//if target hit is a cube from the APlaneActor class
	if (UKismetMathLibrary::ClassIsChildOf(hitActorClass, APlaneActor::StaticClass()))
	{
		APlaneActor* PA = Cast<APlaneActor>(hitTestResult.GetActor());
	}
}

bool ACustomARPawn::WorldHitTest(const FVector2D& screenPos, FHitResult& hitResult)
{
	//Get Player Controller
	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);

	//Perform deprojection taking 2d clicked area generating reference in 3d world space.
	FVector outWorldPosition;
	FVector outWorldDirection;
	bool deprojectionSuccess = UGameplayStatics::DeprojectScreenToWorld(playerController, screenPos, outWorldPosition, outWorldDirection);

	//construct trace vector from point clicked to 1000.0 units beyond in same direction
	FVector traceVector = outWorldDirection * 1000.0;
	traceVector = outWorldPosition + traceVector;

	//perform line trace 
	bool traceSuccess = GetWorld()->LineTraceSingleByChannel(hitResult, outWorldPosition, traceVector, ECollisionChannel::ECC_WorldDynamic);

	//return if the operation was succesful
	return traceSuccess;
}

void ACustomARPawn::OnScreenRelease(const ETouchIndex::Type fingerIndex, const FVector screenPos)
{
	
	//when touch is released store position on screen
	finalTouchLoc = screenPos;

	//get difference between intial touch and final touch
	touchDifference = initalTouchLoc - finalTouchLoc;		
	
}

void ACustomARPawn::FindCandidateImage(float DeltaTime)
{
	auto trackedImages = UARBlueprintLibrary::GetAllTrackedImages();

	for (int i = 0; i < trackedImages.Num(); i++)
	{
		if (trackedImages[i]->GetDetectedImage())
		{
			//if an van gogh is detected then spawn a cube
			if (trackedImages[i]->GetDetectedImage()->GetFriendlyName().Equals("van_gogh"))
			{
				if (!bGoghFound && !bMovingCubeFound)
				{
					bGoghFound = true;
					bSpawnCube = true;
					//Spawn Cube at Image location
					SpawnObject(trackedImages[i]->GetLocalToTrackingTransform().GetLocation());
				}	

				//get tracked object transform
				Tf = trackedImages[i]->GetLocalToTrackingTransform();
				Tf.SetScale3D(FVector(0.1f));				

				if (GoghCube && !bMovingCubeFound)
				{					
					SpawnLocGogh = Tf.GetLocation();
				}
			}
						
			//if an earth image is detected then spawn a sphere
			if (trackedImages[i]->GetDetectedImage()->GetFriendlyName().Equals("earth"))
			{
				if (!bEarthFound && !bMovingCubeFound)
				{
					bEarthFound = true;
					bSpawnCube = false;
					SpawnObject(trackedImages[i]->GetLocalToTrackingTransform().GetLocation());
				}

				//get tracked object transform
				Tf2 = trackedImages[i]->GetLocalToTrackingTransform();
				Tf2.SetScale3D(FVector(0.1f));

				if (EarthSphere && !bMovingCubeFound)
				{					
					EarthSphere->MyStaticMeshComponent->SetWorldScale3D(FVector(0.05f));
					EarthSphere->LemniscateMovement(DeltaTime, Tf2.GetLocation());

					//GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, "Earth Sphere Valid");

					FVector SpawnLoc = Tf2.GetLocation();
				}				
			}

			if (bGoghFound && bEarthFound)
			{
				if (!bMovingCubeFound)
				{
					EarthSphere->Destroy();
					GoghCube->Destroy();

					bMovingCubeFound = true;

					//set position and rotation of spawning object
					FActorSpawnParameters SpawnInfo;
					FRotator myRot(0, 0, 0);
					FVector myLoc(0, 0, 0);

					//Spawn Cube that will move between two images
					MovingCube = GetWorld()->SpawnActor<AEnemyCubeActor>(myLoc, myRot, SpawnInfo);
				}

				if (MovingCube)
				{
					MovingCube->MyStaticMeshComponent->SetWorldScale3D(FVector(0.05f));
					MovingCube->MoveBetweenTwoImages(Tf.GetLocation(), Tf2.GetLocation(), DeltaTime);

					FVector SpawnLoc = MovingCube->GetActorLocation();

				}
			}
		}
	}
}

