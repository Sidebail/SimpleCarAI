// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_CaravanPawn.h"


#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ACPP_CaravanPawn::ACPP_CaravanPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/* Initializing the Components */
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneRoot;

	CaravanMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CaravanMesh"));
	CaravanMesh->SetCollisionProfileName(TEXT("NoCollision"));
	CaravanMesh->SetGenerateOverlapEvents(false);
	CaravanMesh->SetupAttachment(RootComponent);

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("Direction"));
	ArrowComponent->SetupAttachment(RootComponent);

	BoundBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CaravanBoundaries"));
	BoundBox -> SetBoxExtent(FVector(32.f, 32.f, 32.f));
	BoundBox -> SetGenerateOverlapEvents(true);
	BoundBox -> SetCollisionProfileName(TEXT("Pawn"));
	BoundBox->SetupAttachment(ArrowComponent);
}

// Called when the game starts or when spawned
void ACPP_CaravanPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACPP_CaravanPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveForwardByTick(DeltaTime);
	TraceBoxObstacleDetection();
}

// Called to bind functionality to input
void ACPP_CaravanPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACPP_CaravanPawn::MoveForwardByTick(float DeltaSeconds)
{
	AddActorLocalOffset(FVector(Speed * DeltaSeconds, 0 , 0));
}

void ACPP_CaravanPawn::TraceBoxObstacleDetection()
{
	

	ObstacleDetected = GetWorld()->SweepSingleByChannel(TraceResults, ArrowComponent->GetComponentLocation(),
                ArrowComponent->GetComponentLocation() + (ArrowComponent->GetForwardVector() * SpottingDistance),
                GetActorRotation().Quaternion(),ECC_Visibility,
                BoundBox->GetCollisionShape());

	if(ObstacleDetected)
	{
		ObstacleAvoided = false;

		// Printing out that caravan detects obstacle!
		
		if(GEngine) 
			GEngine->AddOnScreenDebugMessage(
                -1, 0.1f, FColor::Yellow,
                FString::Printf(TEXT("Obstacle! - %d"), TraceResults.IsValidBlockingHit())
            );
        
	}
	
	
}

void ACPP_CaravanPawn::SmoothRotateToTarget(float DeltaTime, FVector TargetLocation)
{
	SetActorRotation(
		
		FMath::RInterpTo(
			GetActorRotation(),
			UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetLocation),
			DeltaTime,
			TurningRate * DeltaTime
		)
	);

	
}

void ACPP_CaravanPawn::TurnByTick(float DeltaTime)
{
	AddActorLocalRotation(FRotator(0,TurningRate * DeltaTime * RandomTurn, 0));
}

