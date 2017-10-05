// Fill out your copyright notice in the Description page of Project Settings.

#include "FebruaryGame.h"
#include "AIController_Enemy.h"
#include "Actor_EnemyEscapeZone.h"

void AAIController_Enemy::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	InPawn->SetOwner(this);

	FVector target(-999999999);

	for (TActorIterator<AActor_EnemyEscapeZone> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		target = ActorItr->GetActorLocation();
	}

	if (!target.Equals(FVector(-999999999)))
	{
		MoveToLocation(target);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Error, EnemyEscapeZone does not exist in world!"));
	}
}
