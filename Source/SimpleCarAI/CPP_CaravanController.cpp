// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_CaravanController.h"

#include "Kismet/KismetMathLibrary.h"

ACPP_CaravanController::ACPP_CaravanController()
{
}

void ACPP_CaravanController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    
    ControlledCaravan = Cast<ACPP_CaravanPawn>(InPawn);
    if(ControlledCaravan)
    {
        Checkpoints = ControlledCaravan->Checkpoints;
        CurrentMovementTargetLocation = Checkpoints[CheckpointCurrentIndex]->GetActorLocation();
    }
    
}

void ACPP_CaravanController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    if(ControlledCaravan)
    {
        ControlledCaravan->MoveForwardByTick(DeltaSeconds);
        ControlledCaravan->TraceBoxObstacleDetection();
        if(ControlledCaravan->ObstacleDetected)
        {
            ControlledCaravan->TurnByTick(DeltaSeconds);
        }
        else
        {
            if(!ControlledCaravan->ObstacleAvoided)
            {
                bIsLost = true;
                CurrentMovementTargetLocation = ControlledCaravan->GetActorLocation() + ControlledCaravan->GetActorForwardVector() * ControlledCaravan->SpottingDistance;
                ControlledCaravan->ObstacleAvoided = true;
            }
            ControlledCaravan->SmoothRotateToTarget(DeltaSeconds, CurrentMovementTargetLocation);
            CheckTargetApproach();
        }
    }
        
}

void ACPP_CaravanController::CheckTargetApproach()
{
    
    if(GEngine)
        GEngine->AddOnScreenDebugMessage(
            -1, 2.1f, FColor::Green,
            FString::Printf(TEXT("%f"), UKismetMathLibrary::Vector_Distance(ControlledCaravan->GetActorLocation(), CurrentMovementTargetLocation) )
        );
    if(UKismetMathLibrary::Vector_Distance(ControlledCaravan->GetActorLocation(), CurrentMovementTargetLocation) <= ControlledCaravan->ArrivalConfirmationDistance)
    {
        if(GEngine)
            GEngine->AddOnScreenDebugMessage(
                -1, 2.1f, FColor::Yellow,
                FString::Printf(TEXT("ARRIVED!"))
            );
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
