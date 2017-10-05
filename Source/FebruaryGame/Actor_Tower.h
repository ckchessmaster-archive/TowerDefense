// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "UClass_Globals.h"

#include "Actor_Tower.generated.h"

// need to add attack type enum
UENUM(BlueprintType)
enum class EAttackType : uint8
{
	AT_Beam			UMETA(DisplayName="Beam"),
	AT_AOESphere	UMETA(DisplayName="AOE-Sphere"),
	AT_AOELine		UMETA(DisplayName="AOE-Line")
};

UCLASS()
class FEBRUARYGAME_API AActor_Tower : public AActor
{
	GENERATED_BODY()

	UFUNCTION()
	void OnBeginAttackOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndAttackOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	bool CanSpawn = false;
	bool IsSpawned = false;
	bool bWeaponFired = false;
	FTimerHandle AttackTimerHandle;

protected:

	UFUNCTION(BlueprintNativeEvent, Category = Attack)
	void Attack();
	virtual void Attack_Implementation();

	// Helper function to let blueprint know we have just switched to a new target
	UFUNCTION(BlueprintImplementableEvent, Category = Info)
	void NewTargetNotify();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Config)
	FName TowerType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Config)
	TArray<EStatusEffect> StatusEffects;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Config)
	int32 GridHeight = 2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Config)
	int32 GridWidth = 2;

	// The target to attack
	UPROPERTY(BlueprintReadOnly, Category = Info)
	AActor* Target;

public:	
	// Sets default values for this actor's properties
	AActor_Tower();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION(BlueprintNativeEvent, Category = Misc)
	void SpawnTower();
	void SpawnTower_Implementation();

	// Sells the tower for half the money it was bought for
	UFUNCTION()
	void SellTower();

	UFUNCTION(BlueprintPure, Category = Getters)
	int32 GetTowerCost();

	UFUNCTION(BlueprintPure, Category = Getters)
	bool GetIsSpawned();

	UFUNCTION(BlueprintPure, Category = Getters)
	bool GetCanSpawn();

	UFUNCTION(BlueprintNativeEvent, Category = StateChange)
	void SetCanSpawn(bool AbleToSpawn);
	void SetCanSpawn_Implementation(bool AbleToSpawn);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	FName TowerName;
	float BaseDamage;
	float Cooldown = 1.0f;
	EAttackType AttackType;
	int32 TowerCost = 25.0f;
	int32 IncomeGain = 10.0f;
	EDamageType DamageType;
	float Drain;
	float BonusStat;
	float SlowPercent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Config)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Config)
	USphereComponent* AttackRange;
	
};
