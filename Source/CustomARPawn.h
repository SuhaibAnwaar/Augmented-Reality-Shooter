// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CustomARPawn.generated.h"

class UCameraComponent;
class UMaterial;
class AEnemyCubeActor;
class ABulletSphereActor;
class ACustomActor;
class UARSessionConfig;
class APlaneActor;

UCLASS()
class ARSETUPTUTORIAL_API ACustomARPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACustomARPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void SetMeshMaterial(bool isCube);

	virtual void DisplayCameraInfo();

	virtual void SpawnObject(FVector InImageLoc);

	virtual void OnScreenTouch(const ETouchIndex::Type fingerIndex, const FVector screenPos);

	virtual bool WorldHitTest(const FVector2D& screenPos, FHitResult& hitResult);

	virtual void OnScreenRelease(const ETouchIndex::Type fingerIndex, const FVector screenPos);

	virtual void FindCandidateImage(float DeltaTime);

	//Compoenents
	UPROPERTY(Category = "myCategory", VisibleAnywhere, BlueprintReadWrite)
		USceneComponent* SceneComponent;

	UPROPERTY(Category = "myCategory", VisibleAnywhere, BlueprintReadWrite)
		UCameraComponent* CameraComponent;

	//variables
	UPROPERTY(Category = "myCategory", VisibleAnywhere, BlueprintReadWrite)
		UMaterial* WhiteMaterial;

	UPROPERTY(Category = "myCategory", VisibleAnywhere, BlueprintReadWrite)
		UMaterial* RedMaterial;

	FTimerHandle cameraTicker;
	float cameraNotifyLoopTime;

	FTimerHandle cubeTicker;

	//Y position of the cube
	float ObjectYPos;

	bool bSpawnCube;

	bool isCubeClicked;
	bool isSphereClicked;

	//Touch interactivity variables
	FVector initalTouchLoc = FVector(0.0f, 0.0f, 0.0f);
	FVector finalTouchLoc = FVector(0.0f, 0.0f, 0.0f);
	FVector touchDifference = FVector(0.0f, 0.0f, 0.0f);

	AEnemyCubeActor* enemyCubeActorRef;
	ABulletSphereActor* bulletSphereActorRef;

	//images Tracking Variables
	UARSessionConfig* globalConfig;

	bool bGoghFound;
	ACustomActor* GoghCube;

	bool bEarthFound;
	ABulletSphereActor* EarthSphere; 

	bool bMovingCubeFound;
	AEnemyCubeActor* MovingCube;

	FTransform Tf;
	FTransform Tf2;

	//the previous position of gogh cube
	FVector SpawnLocGogh;

	float Scanning;

	APlaneActor* PlaneActor;
	bool bPlaneActorFound;

	FVector2D ScreenPos;

	bool bScreenTouched;

};