// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CPP_CaravanPawn.h"

#include "CPP_CaravanController.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLECARAI_API ACPP_CaravanController : public AAIController
{
	GENERATED_BODY()

public:
    ACPP_CaravanController();

    ACPP_CaravanPawn* ControlledCaravan = nullptr;

    TArray<ATargetPoint*> Checkpoints;

    FVector CurrentMovementTargetLocation;

    int32 CheckpointCurrentIndex;
    
    virtual void OnPossess(APawn* InPawn) override;

    virtual void Tick(float DeltaSeconds) override;

    void CheckTargetApproach();

    bool bIsLost = false;
    
};
