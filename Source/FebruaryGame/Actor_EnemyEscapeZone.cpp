// Fill out your copyright notice in the Description page of Project Settings.

#include "FebruaryGame.h"
#include "Actor_EnemyEscapeZone.h"
#include "GameState_Main.h"
#include "Character_Enemy.h"
#include "AIController.h"
#include "Actor_EnemySpawnZone.h"

// Sets default values
AActor_EnemyEscapeZone::AActor_EnemyEscapeZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	OverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxOverlap"));
	RootComponent = OverlapBox;
	
	OnActorBeginOverlap.AddDynamic(this, &AActor_EnemyEscapeZone::OnOverlapBegin);
}

void AActor_EnemyEscapeZone::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<AActor_EnemySpawnZone> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		SpawnLocation = ActorItr->GetActorLocation();
	}
}

void AActor_EnemyEscapeZone::OnOverlapBegin(class AActor* OtherActor)
{
	ACharacter_Enemy* convertedActor = Cast<ACharacter_Enemy>(OtherActor);

	if (convertedActor)
	{
		// Remove a life
		Cast<AGameState_Main>(GetWorld()->GetGameState())->AddRemoveLife(true);
		// Teleport back to spawn
		convertedActor->SetActorLocation(SpawnLocation);
		// Start moving back towards escape zone
		Cast<AAIController>(convertedActor->GetOwner())->MoveToLocation(GetActorLocation());

	}
}