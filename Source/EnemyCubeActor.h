// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyCubeActor.generated.h"

UCLASS()
class ARSETUPTUTORIAL_API AEnemyCubeActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemyCubeActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;

	virtual void LemniscateMovement(float DeltaTime, FVector InitialLoc);

	virtual void MoveBetweenTwoImages(FVector ImgLoc1, FVector ImgLoc2, float DeltaTime);

	UPROPERTY(Category = "myCategory", VisibleAnywhere, BlueprintReadWrite)
		USceneComponent* SceneComponent;

	UPROPERTY(Category = "myCategory", VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* MyStaticMeshComponent;

	float T;

	FVector initialLocation;

	FVector StartLoc;

	FVector EndLoc;

	bool isIncrementT;
};