// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "UClass_Globals.h"

#include "FebruaryGameGameMode.generated.h"

UENUM(BlueprintType)
enum class EGameType : uint8
{
	GT_Normal			UMETA(DisplayName="Normal"),
	GT_Extreme			UMETA(DisplayName="Extreme"),
	GT_Infinite			UMETA(DisplayName="Infinite"),
	GT_InfiniteExtreme	UMETA(DisplayName="Infinite-Extreme")
};

UCLASS()
class FEBRUARYGAME_API AFebruaryGameGameMode : public AGameMode
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void HandleMatchHasEnded() override;

	// Look for win conditions or prepare infinite mode if available
	void HandleFinalWave();

	// Spawn a new wave
	UFUNCTION()
	void SpawnWave();

	// Spawn the next unit in the wave
	UFUNCTION()
	void SpawnUnit();

	// Only used in infinite mode
	UFUNCTION()
	void SpawnUnit_Infinite();

	UFUNCTION()
	void GiveIncome();

	// Timers
	FTimerHandle WaveTimer;
	FTimerHandle InfiniteSpawnTimer;
	FTimerHandle UnitSpawnTimer;
	FTimerHandle IncomeTimer;

	FVector SpawnLocation;

	int32 WaveUnitIndex = 0;
	
protected:

	UFUNCTION(BlueprintImplementableEvent, Category = StateChange)
	void HandleGameover();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Config)
	EGameType GameType;

};
