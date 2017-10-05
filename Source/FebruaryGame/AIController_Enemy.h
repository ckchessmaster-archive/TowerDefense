// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "AIController_Enemy.generated.h"

/**
 * 
 */
UCLASS()
class FEBRUARYGAME_API AAIController_Enemy : public AAIController
{
	GENERATED_BODY()

	virtual void Possess(APawn* InPawn) override;
	
};
