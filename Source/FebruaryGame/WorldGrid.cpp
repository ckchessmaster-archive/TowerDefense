// Fill out your copyright notice in the Description page of Project Settings.

#include "FebruaryGame.h"
#include "WorldGrid.h"

#include "UClass_GLobals.h"

UWorldGrid::UWorldGrid()
{

}

void UWorldGrid::InitGrid(int32 Size)
{
	// Outer loop adds arrays that the inner loop populates with cells
	for (int32 x = 0; x < Size; x++)
	{
		// Create a new array to hold our coordinates
		FWorldGridYCoords newYCoord;
		WorldGrid.Add(newYCoord);

		for (int y = 0; y < Size; y++)
		{
			// Create a new cell
			FGridCell newCell;
			newCell.IsOccupied = false;
			newCell.WorldCoordinates = FVector(x * GridScale, y * GridScale, 0);

			// Add the new cell to the grid
			WorldGrid[x].WorldGridCells.Add(newCell);
		}
	}

	IsGridInitialized = true;
}

void UWorldGrid::LoadMapFromFile(FString Filename)
{
	// Load the map into a string
	FString completeFilePath = FPaths::GameDir() + "Content/Main/MapTemplates/" + Filename + ".txt";
	FString fileData = "";
	FFileHelper::LoadFileToString(fileData, *completeFilePath);

	// Split the string into an array for easier parsing
	TArray<FString> lines;
	int32 lineCount = fileData.ParseIntoArray(lines, _T("\n"), true);

	if (lineCount == 0)
	{
		UE_LOG(LogTemp, Fatal, TEXT("The map failed to load cannot continue!"));
		return;
	}

	if (lines.Num() != (lines[0].Len() - 1)) // Minus two because of end of line charachters \r and \O
	{
		UE_LOG(LogTemp, Fatal, TEXT("The map is not of uniform size cannot continue!"));
		return;
	}

	// Initialize the world grid
	InitGrid(lineCount);

	// Mark cells as occupied
	for (int32 x = 0; x < lineCount; x++)
	{
		for (int32 y = 0; y < (lineCount - 2); y++) // Minus two because of end of line charachters \r and \O
		{
			if (lines[x][y] == 'O')
			{
				WorldGrid[x].WorldGridCells[y].IsOccupied = false;
			}
			else if (lines[x][y] == 'X')
			{
				WorldGrid[x].WorldGridCells[y].IsOccupied = true;
			}
			else
			{
				UE_LOG(LogTemp, Fatal, TEXT("The map contains an illegal character cannot continue!"));
				return;
			}
		}
	}

}

void UWorldGrid::AlignActorToGrid(AActor* Actor, int32 Height, int32 Width)
{
	if (IsGridInitialized && WorldGrid.IsValidIndex(0))
	{
		// Center actor with grid cells
		FVector coordinates = WorldGrid[0].WorldGridCells[0].WorldCoordinates;
		coordinates.X = coordinates.X + (GridScale / Height);
		coordinates.Y = coordinates.Y + (GridScale / Width);

		// Move actor to the grid
		Actor->SetActorLocation(coordinates);
		AlignedActors.Add(Actor);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("The world grid has not been initialized yet!"));
	}
}

FVector UWorldGrid::GetClosestGridCoordinates(FVector Location, int32 Height, int32 Width)
{
	if (IsGridInitialized && WorldGrid.Num() > 0)
	{
		// Divide by cell size and round down to get closest cell
		int32 xCoord = FMath::DivideAndRoundDown<int32>(Location.X, GridScale);
		int32 yCoord = FMath::DivideAndRoundDown<int32>(Location.Y, GridScale);

		// Make sure neither of the coordinates is less than 0
		if (xCoord < 0)
		{
			xCoord = 0;
		}
		if (yCoord < 0)
		{
			yCoord = 0;
		}
		// Make sure neither of the coordinates is greater than the size of the array minus the size of the actor in grid units
		if (xCoord > (WorldGrid.Num() - Height))
		{
			xCoord = WorldGrid.Num() - Height;
		}
		if (yCoord > (WorldGrid.Num() - Width))
		{
			yCoord = WorldGrid.Num() - Width;
		}

		// Center actor with grid cells
		FVector coordinates = WorldGrid[xCoord].WorldGridCells[yCoord].WorldCoordinates;
		coordinates.X = coordinates.X + (GridScale / Height);
		coordinates.Y = coordinates.Y + (GridScale / Width);

		return coordinates;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("The world grid has not been initialized yet!"));
	}

	return FVector(999999999999);
}

bool UWorldGrid::IsGridAreaFree(FVector Location, int32 Height, int32 Width)
{
	if (IsGridInitialized)
	{
		// Divide by cell size and round down to get closest cell
		int32 xCoord = FMath::DivideAndRoundDown<int32>(Location.X, GridScale);
		int32 yCoord = FMath::DivideAndRoundDown<int32>(Location.Y, GridScale);

		// Make sure neither of the coordinates is less than 0
		if (xCoord < 0)
		{
			xCoord = 0;
		}
		if (yCoord < 0)
		{
			yCoord = 0;
		}
		// Make sure neither of the coordinates is greater than the size of the array minus the size of the actor in grid units
		if (xCoord >(WorldGrid.Num() - Height))
		{
			xCoord = WorldGrid.Num() - Height;
		}
		if (yCoord > (WorldGrid.Num() - Width))
		{
			yCoord = WorldGrid.Num() - Width;
		}

		// Check if any of the coordinates in the area are occupied
		for (int32 x = 0; x < Height; x++)
		{
			for (int32 y = 0; y < Width; y++)
			{
				int32 xFinal = xCoord + x;
				int32 yFinal = yCoord + y;
				if (WorldGrid[xFinal].WorldGridCells[yFinal].IsOccupied)
				{
					return false;
				}
			}
		}

		// Return true if all of the cells are unoccupied
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("The world grid has not been initialized yet!"));
	}

	return false;
}

bool UWorldGrid::SetCellsAsOccupied(int32 XCoord, int32 YCoord, bool IsOccupied)
{
	if (XCoord < 0 || YCoord < 0 || (XCoord > WorldGrid.Num() - 1) || (YCoord > WorldGrid.Num() - 1))
	{
		UE_LOG(LogTemp, Warning, TEXT("The given coordinates do not exist on the world grid!"));
		return false;
	}

	WorldGrid[XCoord].WorldGridCells[YCoord].IsOccupied = IsOccupied;

	return true;
}

bool UWorldGrid::SetCellsAsOccupied(AActor* OccupyingActor, bool IsOccupied, int32 Height, int32 Width)
{
	if (!AlignedActors.Contains(OccupyingActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("The given actor is not aligned with the world grid!"));
		return false;
	}

	FVector location = OccupyingActor->GetActorLocation();
	int32 xCoord = FMath::DivideAndRoundDown<int32>(location.X, GridScale);
	int32 yCoord = FMath::DivideAndRoundDown<int32>(location.Y, GridScale);

	for (int32 x = 0; x < Height; x++)
	{
		for (int32 y = 0; y < Width; y++)
		{
			int32 xFinal = xCoord + x;
			int32 yFinal = yCoord + y;
			WorldGrid[xFinal].WorldGridCells[yFinal].IsOccupied = IsOccupied;
		}
	}

	return true;
}
