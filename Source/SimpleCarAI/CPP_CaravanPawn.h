// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "GameFramework/Pawn.h"
#include "CPP_CaravanPawn.generated.h"

UCLASS()
class SIMPLECARAI_API ACPP_CaravanPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACPP_CaravanPawn();

	/* ACTOR COMPONENTS */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta =(AllowPrivateAccess="true"), Category= Components)
		class USceneComponent* SceneRoot = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta =(AllowPrivateAccess="true"), Category= Components)
		class UArrowComponent* ArrowComponent = nullptr;

	// Editable and visible for GameDesigner
	UPROPERTY( BlueprintReadWrite, meta =(AllowPrivateAccess="true"), Category= Components, EditAnywhere)
		class UStaticMeshComponent* CaravanMesh = nullptr;
	UPROPERTY(BlueprintReadWrite, meta =(AllowPrivateAccess="true"), Category= Components, EditAnywhere)
		class UBoxComponent* BoundBox = nullptr;

	/* VARIABLES */

	/* Movement */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float Speed = 1500.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float TurningRate = 160.0f;

	/* Perception */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception")
		FVector BoxTraceHalfSize  = FVector(0,0,0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception")
		FHitResult TraceResults = FHitResult();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception")
		bool ObstacleDetected = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception")
		bool ObstacleAvoided = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception")
		FVector AvoidancePoint = FVector(0,0,0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception")
		float SpottingDistance = 1000;

	/* Checkpoint Navigation */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI_Navigation")
		TArray<ATargetPoint*> Checkpoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI_Navigation")
		float ArrivalConfirmationDistance = 50;

	/* Helper Variables */

	int32 RandomTurn = 1;
		

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/* !FUNCTIONS! */
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/*
	 * Adds Actor Local Offset every Tick by Speed X DeltaSeconds on X vector
	 */
	UFUNCTION(BlueprintCallable)
		void MoveForwardByTick(float DeltaSeconds);

	/*
	 * Casts a trace box in front of the caravan with its boundaries and SpottingDistance
	 * Saves TraceResults and sets ObstacleDetected
	 * If ObstacleDetected = true, then sets ObstacleAvoided to false
	 */
	UFUNCTION(BlueprintCallable)
		void TraceBoxObstacleDetection();

	/*
	 * Smoothly changes Caravan's rotation to face the target location
	 * Uses interpolation (RInterp) and SetActorRotation()
	 */
	UFUNCTION(BlueprintCallable)
		void SmoothRotateToTarget(float DeltaTime, FVector TargetLocation);

	/*
	 * Constantly adding LocalRotation so caravan turns right
	 * Used for avoiding obstacles
	 *
	 * OKAY, its just a tape-work made to avoid proper avoidance logic, that would calculate the shortest path.
	 * But for that I'd really consider a proper AI with BehaviourTree in UE4
	 */
	UFUNCTION(BlueprintCallable)
		void TurnByTick(float DeltaTime);
};
