// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Actor_EnemyEscapeZone.generated.h"

UCLASS()
class FEBRUARYGAME_API AActor_EnemyEscapeZone : public AActor
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(class AActor* OtherActor);

	FVector SpawnLocation;

public:	
	// Sets default values for this actor's properties
	AActor_EnemyEscapeZone();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UBoxComponent* OverlapBox;
	
};
