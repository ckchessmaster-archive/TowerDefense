// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UClass_Globals.generated.h"

UENUM(BlueprintType)
enum class EDamageType : uint8
{
	DT_None			UMETA(DisplayName = "None"),
	DT_White		UMETA(DisplayName = "White"),
	DT_Red			UMETA(DisplayName = "Red"), 
	DT_Green		UMETA(DisplayName = "Green"),
	DT_Blue			UMETA(DisplayName = "Blue")
};

UENUM(BlueprintType)
enum class EUnitType : uint8
{
	UT_None			UMETA(DisplayName = "None"),
	UT_HP			UMETA(DisplayName = "Tank"),
	UT_Speed		UMETA(DisplayName = "Racer"),
	UT_BShield		UMETA(DisplayName = "BlueShielder"),
	UT_RShield		UMETA(DisplayName = "RedShielder"),
	UT_GShield		UMETA(DisplayName = "GreenShielder"),
	UT_Splitter		UMETA(DisplayName = "Splitter"),
	UT_Healer		UMETA(Displayname = "Healer"),
	UT_Drainer		UMETA(DisplayName = "Vampire"),
	UT_Special		UMETA(DisplayName = "Special"),
	UT_Boss			UMETA(DisplayName = "Boss")
};

USTRUCT(BlueprintType)
struct FTowerData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	FTowerData() : DamageAmount(0), Cooldown(0), BonusStat(0), SlowPercent(0), Drain(0) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TowerData)
	float DamageAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TowerData)
	float Cooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TowerData)
	float AttackRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TowerData)
	float BonusStat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TowerData)
	float SlowPercent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TowerData)
	float Drain;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TowerData)
	EDamageType DamageType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TowerData)
	int32 TowerCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TowerData)
	int32 IncomeGain;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TowerData)
	FString AssetPath;
};

USTRUCT(BlueprintType)
struct FWaveData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	FWaveData() : HP(0), Speed(0) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WaveData)
	int32 HP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WaveData)
	float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WaveData)
	EUnitType UnitType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WaveData)
	FString AssetPath;
};

UENUM(BlueprintType)
enum class EStatusEffect : uint8
{
	SE_None			UMETA(DisplayName="None"),
	SE_Poisen		UMETA(DisplayName="Poisen"),
	SE_Slow			UMETA(DisplayName="Slow")
};

UCLASS()
class FEBRUARYGAME_API UClass_Globals : public UObject
{
	GENERATED_BODY()

public:
	
	// Load Blueprint from path (there must be one already in the level, also might not work with packaged project, need to fix this later)
	static FORCEINLINE UClass* LoadBlueprintClassFromPath(const FName& Path)
	{
		if (Path == NAME_None) return NULL;

		FString finalPath = RootPath + Path.ToString();

		return LoadObject<UClass>(nullptr, *finalPath);
	}

	// The path of the game
	static const FString RootPath;
};
