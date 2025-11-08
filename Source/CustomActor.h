// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CustomActor.generated.h"

class ACustomARPawn;

UCLASS()
class ARSETUPTUTORIAL_API ACustomActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACustomActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;

	virtual void LemniscateMovement(float DeltaTime, FVector InitialLoc);

	virtual void MoveTowardsPlayer(FVector PlayerStartingPos);

	//When the cube overlaps the pawn
	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(Category = "myCategory", VisibleAnywhere, BlueprintReadWrite)
		USceneComponent* SceneComponent;

	UPROPERTY(Category = "myCategory", VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* MyStaticMeshComponent;

	float T;

	FVector initialLocation;

	//the current lerp value along the vector between two points
	float LerpMultiplier;

	ACustomARPawn* PlayerRef;
};
