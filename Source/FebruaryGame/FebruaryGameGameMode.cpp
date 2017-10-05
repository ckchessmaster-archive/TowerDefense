// Fill out your copyright notice in the Description page of Project Settings.

#include "FebruaryGame.h"
#include "FebruaryGameGameMode.h"

#include "GameState_Main.h"
#include "Actor_EnemySpawnZone.h"
#include "Character_Enemy.h"

#include "UClass_Globals.h"


void AFebruaryGameGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Set the wave spawn location
	for (TActorIterator<AActor_EnemySpawnZone> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		SpawnLocation = ActorItr->GetActorLocation();
	}

	// Load wave table
	FString tablePath = "DataTable'/Game/Main/Waves.Waves'";
	GetGameState<AGameState_Main>()->WaveTable = LoadObject<UDataTable>(nullptr, *tablePath);

	// Setup timers
	GetWorldTimerManager().SetTimer(WaveTimer, this, &AFebruaryGameGameMode::SpawnWave, 30.0f, true, 5.0f);
	GetWorldTimerManager().SetTimer(UnitSpawnTimer, this, &AFebruaryGameGameMode::SpawnUnit, 0.5f, true, 0.0f);
	GetWorldTimerManager().SetTimer(IncomeTimer, this, &AFebruaryGameGameMode::GiveIncome, 15.0f, true, 15.0f);

	// This timer only runs when a wave is spawning
	GetWorldTimerManager().PauseTimer(UnitSpawnTimer);
}

void AFebruaryGameGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Check if we have run out of lives
	if (GetGameState<AGameState_Main>()->GetLives() <= 0)
	{
		HandleGameover();
	}

	// Check if we are on the final wave
	if (GetGameState<AGameState_Main>()->GetCurrentWave() == 50)
	{
		HandleFinalWave();
	}

	// Update time
	GetGameState<AGameState_Main>()->UpdateTime(DeltaSeconds);

	// Check if we have spawned the max number of units for this wave
	if (WaveUnitIndex >= 10)
	{
		GetWorldTimerManager().PauseTimer(UnitSpawnTimer);
		WaveUnitIndex = 0;
	}
}

void AFebruaryGameGameMode::HandleMatchHasEnded()
{
	Super::HandleMatchHasEnded();


}

void AFebruaryGameGameMode::HandleFinalWave()
{
	// Clear the wave timer as we don't need it any more
	if (GetWorldTimerManager().IsTimerActive(WaveTimer))
	{
		GetWorldTimerManager().ClearTimer(WaveTimer);
	}

	// Special handeling of infinite modes
	if ((GameType == EGameType::GT_Infinite || GameType == EGameType::GT_InfiniteExtreme) && !GetWorldTimerManager().IsTimerActive(InfiniteSpawnTimer))
	{
		GetWorldTimerManager().SetTimer(InfiniteSpawnTimer, this, &AFebruaryGameGameMode::SpawnUnit_Infinite, 1.0f, true, 30.0f);
	}

	// Check to see if all enemies are dead
	for (TActorIterator<ACharacter_Enemy> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		// If an enemy exists continue the game otherwise end game with a win condition
		if (ActorItr)
		{
			break;
		}
		else
		{
			EndMatch();
		}
	}
}

// @ToDo write up wave spawning stuff later
void AFebruaryGameGameMode::SpawnWave()
{
	// Increment the current wave number
	GetGameState<AGameState_Main>()->NextWave();
	GetWorldTimerManager().UnPauseTimer(UnitSpawnTimer);
}

void AFebruaryGameGameMode::SpawnUnit()
{
	UClass* unitClass = nullptr;

	// Subtract 1 from the gamestate's version so that we can acess the wave array easier
	FString currentWave = "Wave" + FString::FromInt(GetGameState<AGameState_Main>()->GetCurrentWave());
	FString contextString = "";
	FWaveData* unitData = GetGameState<AGameState_Main>()->WaveTable->FindRow<FWaveData>(*currentWave, contextString, true);

	unitClass = UClass_Globals::LoadBlueprintClassFromPath(*unitData->AssetPath);
	
	// Spawn the unit
	FTransform transform;
	transform.SetLocation(SpawnLocation);
	ACharacter_Enemy* actorToSpawn = GetWorld()->SpawnActorDeferred<ACharacter_Enemy>(unitClass, FTransform());

	// Set initial parameters
	actorToSpawn->SetMaxHP(unitData->HP);
	actorToSpawn->GetCharacterMovement()->MaxWalkSpeed = (unitData->Speed);
	actorToSpawn->UnitType = unitData->UnitType;

	actorToSpawn->FinishSpawning(transform);

	// Increment to the next unit
	WaveUnitIndex++;
}

// @todo later
void AFebruaryGameGameMode::SpawnUnit_Infinite()
{
	UClass* unitToSpawn = UClass_Globals::LoadBlueprintClassFromPath(TEXT("Class'/Game/Main/BaseBlueprints/EnemyTemplate.EnemyTemplate_C'"));

	// Spawn the unit
	FTransform transform;
	transform.SetLocation(SpawnLocation);
	ACharacter_Enemy* actorToSpawn = GetWorld()->SpawnActorDeferred<ACharacter_Enemy>(unitToSpawn, FTransform());

	actorToSpawn->FinishSpawning(transform);
}

void AFebruaryGameGameMode::GiveIncome()
{
	GetGameState<AGameState_Main>()->UpdateMoney(GetGameState<AGameState_Main>()->GetIncome());
}
