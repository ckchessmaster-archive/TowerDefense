// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameState.h"
#include "WorldGrid.h"

#include "GameState_Main.generated.h"


UCLASS()
class FEBRUARYGAME_API AGameState_Main : public AGameState
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

	// Grid testing
	void ShowWorldGrid();

	UWorldGrid* WorldGrid;

	int32 Lives = 3;
	float LightLevel = 1800.0f;
	int32 Money = 100;
	int32 Income = 10;
	int32 Score = 0;
	float Time = 0.0f;
	int32 CurrentWave = 0;

public:
	
	void AddRemoveLife(bool RemoveLife);

	UFUNCTION(BlueprintCallable, Category = Utility)
	void UpdateLightLevel(float Amount);

	UFUNCTION(BlueprintCallable, Category = Utility)
	void UpdateMoney(int32 Amount);

	UFUNCTION(BlueprintCallable, Category = Utility)
	void UpdateIncome(int32 Amount);

	void UpdateScore(int32 Amount);
	void UpdateTime(float Amount);
	void NextWave();


	// Getters ------------------------------------
	UFUNCTION(BlueprintPure, Category = Getters)
	UWorldGrid* GetWorldGrid();

	UFUNCTION(BlueprintPure, Category = Getters)
	float GetLightLevel();

	UFUNCTION(BlueprintPure, Category = Getters)
	int32 GetLives();	

	UFUNCTION(BlueprintPure, Category = Getters)
	int32 GetMoney();

	UFUNCTION(BlueprintPure, Category = Getters)
	int32 GetIncome();

	UFUNCTION(BlueprintPure, Category = Getters)
	int32 GetScore();

	UFUNCTION(BlueprintPure, Category = Getters)
	float GetTime();

	UFUNCTION(BlueprintPure, Category = Getters)
	int32 GetCurrentWave();
	// ---------------------------------------------

	// Table containing information on all of the waves in the game
	UDataTable* TowerTable;

	// Table containing information on all of the waves in the game
	UDataTable* WaveTable;
};
