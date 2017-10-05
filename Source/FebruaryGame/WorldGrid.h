// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WorldGrid.generated.h"

// may need to add BlueprintType
USTRUCT()
struct FGridCell
{
	GENERATED_USTRUCT_BODY()

	bool IsOccupied;
	FVector WorldCoordinates;
};

USTRUCT()
struct FWorldGridYCoords
{
	GENERATED_USTRUCT_BODY()

	TArray<FGridCell> WorldGridCells;
};

UCLASS()
class FEBRUARYGAME_API UWorldGrid : public UObject
{
	GENERATED_BODY()

	float GridScale = 100.0f;
	bool IsGridInitialized = false;

	// List of all actors which are alligned with the grid
	TArray<AActor*> AlignedActors;

public:

	UWorldGrid();

	// Initialize the world grid
	void InitGrid(int32 Size);

	void LoadMapFromFile(FString Filename);

	// Align the given actor to the world grid
	void AlignActorToGrid(AActor* Actor, int32 Height=2, int32 Width=2);

	// @return The closest valid grid coordinates based on the size of the object
	FVector GetClosestGridCoordinates(FVector Location, int32 Height=2, int32 Width=2);
	
	/** Checks to see if an area on the grid is free
	 *  @param The location to check
	 *  @param The height of the grid area to check
	 *  @param The width of the grid area to check
	 *  @return true if the given area on the grid is unoccupied
	 */
	UFUNCTION(BlueprintCallable, Category = Info)
	bool IsGridAreaFree(FVector Location, int32 Height, int32 Width);

	// Set the given cell as occupied or unoccupied
	// @return false if the coordinate doesn't exist
	bool SetCellsAsOccupied(int32 XCoord, int32 YCoord, bool IsOccupied);
	// Set the cells taken up by a given actor as occupied or unoccupied
	// @return false if the actor isn't alligned
	bool SetCellsAsOccupied(AActor* OccupyingActor, bool IsOccupied,int32 Height=2, int32 Width=2);
	
	TArray<FWorldGridYCoords> WorldGrid;
};
