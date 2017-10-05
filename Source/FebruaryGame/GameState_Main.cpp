// Fill out your copyright notice in the Description page of Project Settings.

#include "FebruaryGame.h"
#include "GameState_Main.h"
#include "UClass_Globals.h"

void AGameState_Main::BeginPlay()
{
	Super::BeginPlay();

	// Setup world grid/map
	WorldGrid = NewObject<UWorldGrid>();
	WorldGrid->LoadMapFromFile("map1");

	// Used for testing purposes
	//ShowWorldGrid();

	// Load tower table
	FString tablePath = "DataTable'/Game/Main/Towers.Towers'";
	TowerTable = LoadObject<UDataTable>(nullptr, *tablePath);

	Lives = 10;
}

void AGameState_Main::AddRemoveLife(bool RemoveLife)
{
	if (RemoveLife)
	{
		Lives = Lives - 1;
	}
	else
	{
		Lives = Lives + 1;
	}
}

void AGameState_Main::UpdateLightLevel(float Amount)
{
	LightLevel = LightLevel + Amount;
}

void AGameState_Main::UpdateMoney(int32 Amount)
{
	Money = Money + Amount;
}

void AGameState_Main::UpdateIncome(int32 Amount)
{
	Income = Income + Amount;
}

void AGameState_Main::UpdateScore(int32 Amount)
{
	Score = Score + Amount;
}

void AGameState_Main::UpdateTime(float Amount)
{
	Time = Time + Amount;
}

void AGameState_Main::NextWave()
{
	CurrentWave++;
}

// Getters ----------------------------------------------
UWorldGrid* AGameState_Main::GetWorldGrid()
{
	return WorldGrid;
}

float AGameState_Main::GetLightLevel()
{
	return LightLevel;
}

int32 AGameState_Main::GetLives()
{
	return Lives;
}

int32 AGameState_Main::GetMoney()
{
	return Money;
}

int32 AGameState_Main::GetIncome()
{
	return Income;
}

int32 AGameState_Main::GetScore()
{
	return Score;
}

float AGameState_Main::GetTime()
{
	return Time;
}

int32 AGameState_Main::GetCurrentWave()
{
	return CurrentWave;
}
// -------------------------------------------------------

void AGameState_Main::ShowWorldGrid()
{
	UClass* classToSpawn = UClass_Globals::LoadBlueprintClassFromPath("Class'/Game/Main/GridTestActor.GridTestActor_C'");

	for (int x = 0; x < WorldGrid->WorldGrid.Num(); x++)
	{
		for (int y = 0; y < WorldGrid->WorldGrid.Num(); y++)
		{
			FTransform transform;
			transform.SetLocation(WorldGrid->WorldGrid[x].WorldGridCells[y].WorldCoordinates);
			GetWorld()->SpawnActor<AActor>(classToSpawn, transform);
		}
	}
}