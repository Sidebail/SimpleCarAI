// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_CaravanController.h"

#include "Kismet/KismetMathLibrary.h"

ACPP_CaravanController::ACPP_CaravanController()
{
}

void ACPP_CaravanController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    // Setting the Caravan for further usage
    ControlledCaravan = Cast<ACPP_CaravanPawn>(InPawn);
    
    if(ControlledCaravan) // If its valid, then
    {
        Checkpoints = ControlledCaravan->Checkpoints; // Setting the checkpoints in local variable
        CurrentMovementTargetLocation = Checkpoints[CheckpointCurrentIndex]->GetActorLocation(); // And assigning first checkpoint as target!
    }
    
}

void ACPP_CaravanController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    if(ControlledCaravan)
    {
        // Caravan is constantly moving forward and checking for obstacles in front
        ControlledCaravan->MoveForwardByTick(DeltaSeconds);
        ControlledCaravan->TraceBoxObstacleDetection();
        // Turning right or left if obstacle is in front
        if(ControlledCaravan->ObstacleDetected)
        {
            ControlledCaravan->TurnByTick(DeltaSeconds);
        }
        else
        {
            // If Caravan found a path with no obstacles in the way, then set bIsLost to true
            // and set the location in front of him as target in order to avoid the obstacle
            if(!ControlledCaravan->ObstacleAvoided)
            {
                bIsLost = true;
                CurrentMovementTargetLocation = ControlledCaravan->GetActorLocation() + ControlledCaravan->GetActorForwardVector() * ControlledCaravan->SpottingDistance;
                ControlledCaravan->ObstacleAvoided = true;
            }

            // Continue turning to target and checking for arriving
            ControlledCaravan->SmoothRotateToTarget(DeltaSeconds, CurrentMovementTargetLocation);
            CheckTargetApproach();
        }
    }
        
}

void ACPP_CaravanController::CheckTargetApproach()
{
    // Printing out the distance to target for debug purposes
    if(GEngine)
        GEngine->AddOnScreenDebugMessage(
            -1, 2.1f, FColor::Green,
            FString::Printf(TEXT("%f"), UKismetMathLibrary::Vector_Distance(ControlledCaravan->GetActorLocation(), CurrentMovementTargetLocation) )
        );

    
    if(UKismetMathLibrary::Vector_Distance(ControlledCaravan->GetActorLocation(), CurrentMovementTargetLocation) <= ControlledCaravan->ArrivalConfirmationDistance)
    {
        // If we were lost, then set the RandomTurn to 1 or -1 and bIsLost to false
        // If we were not lost, then set the next target from Checkpoints array!
        if(bIsLost)
        {
            bIsLost = false;
            ControlledCaravan->RandomTurn = UKismetMathLibrary::RandomBool() ? 1 : -1;
        }
        else
        {
            CheckpointCurrentIndex++;
        }
        
        CheckpointCurrentIndex = CheckpointCurrentIndex % Checkpoints.Num();

        CurrentMovementTargetLocation = Checkpoints[CheckpointCurrentIndex]->GetActorLocation();
    }
}
