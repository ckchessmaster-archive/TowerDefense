// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Actor_EnemySpawnZone.generated.h"

UCLASS()
class FEBRUARYGAME_API AActor_EnemySpawnZone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActor_EnemySpawnZone();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UBoxComponent* SpawnBox;
	
};
