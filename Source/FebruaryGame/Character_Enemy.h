// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "UClass_Globals.h"
#include "Character_Enemy.generated.h"


UCLASS()
class FEBRUARYGAME_API ACharacter_Enemy : public ACharacter
{
	GENERATED_BODY()
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	virtual void Destroyed() override;

	// Called whenever we take damage
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	void TakeDamage(float DamageAmount); // Used with some timer delegates

	float CurrentHP = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = Config)
	float MaxHP = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = Config)
	int32 MoneyValue = 0.0f;

	int32 NumSplits = 10;

	TArray<EStatusEffect> ActiveEffects;

	// The root for the UI widget
	UPROPERTY(EditDefaultsOnly, Category = UI)
	class USceneComponent* WidgetRoot;
	UPROPERTY(EditDefaultsOnly, Category = UI)
	class UWidgetComponent* HPWidget;

protected:

	// Update any UI elements in blueprint
	UFUNCTION(BlueprintImplementableEvent, Category = UI)
	void UpdateUI();

public:
	// Sets default values for this character's properties
	ACharacter_Enemy();

	

	void AddStatusEffect(EStatusEffect Effect);

	// Remove a status effect based on the effect itself or in some cases a timer
	void RemoveStatusEffect(EStatusEffect Effect);
	void RemoveStatusEffect(FTimerHandle Timer);

	// Getters/Setters ------------------------------------------------------------------------------
	UFUNCTION(BlueprintPure, Category = Getters)
	float GetCurrentHP();

	UFUNCTION(BlueprintPure, Category = Getters)
	float GetMaxHP();

	UFUNCTION()
	void UpdateCurrentHP(float Amount);

	void SetMaxHP(float Amount);

	EUnitType UnitType;
	
	
};
