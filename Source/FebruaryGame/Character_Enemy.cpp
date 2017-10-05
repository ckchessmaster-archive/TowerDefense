// Fill out your copyright notice in the Description page of Project Settings.

#include "FebruaryGame.h"
#include "Character_Enemy.h"
#include "DamageTypes.h"

#include "GameState_Main.h"
#include "WidgetComponent.h"

// Sets default values
ACharacter_Enemy::ACharacter_Enemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setup the UI for this enemy
	WidgetRoot = CreateDefaultSubobject<USceneComponent>(TEXT("WidgetRoot"));
	WidgetRoot->AttachTo(RootComponent);
	HPWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPWidget"));
	HPWidget->AttachTo(WidgetRoot);
}

// Called when the game starts or when spawned
void ACharacter_Enemy::BeginPlay()
{
	Super::BeginPlay();
	
	WidgetRoot->SetWorldRotation(FRotator(90, 180, 0));

	// Reduce max hp based on light level (need to adjust so HP doesn't get set lower than .2)
	float light = GetWorld()->GetGameState<AGameState_Main>()->GetLightLevel();
	SetMaxHP(GetMaxHP() / (light * 0.000555f));

	UpdateCurrentHP(GetMaxHP());
}

// Called every frame
void ACharacter_Enemy::Tick( float DeltaTime )
{
	Super::Tick(DeltaTime);

	if (GetCurrentHP() <= 0)
	{
		Destroy();
	}

	UpdateUI();
}

void ACharacter_Enemy::Destroyed()
{
	Super::Destroyed();

	// If we are a splitter spawn the splits
	if (UnitType == EUnitType::UT_Splitter)
	{
		UClass* unitClass = nullptr;
		FString contextString = "";
		FWaveData* unitData = GetWorld()->GetGameState<AGameState_Main>()->WaveTable->FindRow<FWaveData>(TEXT("SplitterSpawn"), contextString, true);
		unitClass = UClass_Globals::LoadBlueprintClassFromPath(*unitData->AssetPath);
		FTransform transform(GetActorLocation());
		// Now spawn 5 of them
		for (int32 x = 0; x < NumSplits; x++)
		{
			ACharacter_Enemy* splitterSpawn = GetWorld()->SpawnActorDeferred<ACharacter_Enemy>(unitClass, FTransform());
			splitterSpawn->SetMaxHP(unitData->HP);
			splitterSpawn->GetCharacterMovement()->MaxWalkSpeed = unitData->Speed;
			splitterSpawn->UnitType = unitData->UnitType;

			splitterSpawn->FinishSpawning(transform);
		}
	}

	AGameState_Main* gamestate = GetWorld()->GetGameState<AGameState_Main>();
	if (gamestate)
	{
		gamestate->UpdateMoney(MoneyValue);
	}
}

float ACharacter_Enemy::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	FCustomDamageEvent* convertedDamageEvent = (FCustomDamageEvent*)&DamageEvent;
	switch (UnitType) 
	{
	default:
		break;
	case EUnitType::UT_BShield:
	{
		if (convertedDamageEvent->DamageType == EDamageType::DT_Blue)
		{
			DamageAmount = DamageAmount * .5;
		}
		break;
	}
	case EUnitType::UT_RShield:
	{
		if (convertedDamageEvent->DamageType == EDamageType::DT_Red)
		{
			DamageAmount = DamageAmount * .5;
		}
		break;
	}
	case EUnitType::UT_GShield:
	{
		if (convertedDamageEvent->DamageType == EDamageType::DT_Green)
		{
			DamageAmount = DamageAmount * .5;
		}
		break;
	}
	}

	UpdateCurrentHP(-1 * DamageAmount);

	return DamageAmount;
}

void ACharacter_Enemy::TakeDamage(float DamageAmount)
{
	TakeDamage(DamageAmount, FDamageEvent(), nullptr, nullptr);
}

void ACharacter_Enemy::AddStatusEffect(EStatusEffect Effect)
{
	if (!ActiveEffects.Contains(Effect))
	{
		ActiveEffects.Add(Effect);

		switch (Effect)
		{
		case EStatusEffect::SE_Poisen:
		{
			FTimerHandle damageTimer;
			FTimerHandle expirationTimer;
			GetWorldTimerManager().SetTimer(damageTimer, FTimerDelegate::CreateUObject(this, &ACharacter_Enemy::TakeDamage, (GetCurrentHP() * 0.05f)), 1.5f, true);
			GetWorldTimerManager().SetTimer(expirationTimer, FTimerDelegate::CreateUObject(this, &ACharacter_Enemy::RemoveStatusEffect, damageTimer), 4.5f, false);
			break;
		}
		case EStatusEffect::SE_Slow:
		{
			GetCharacterMovement()->MaxWalkSpeed = (GetCharacterMovement()->MaxWalkSpeed * 0.5f);
			FTimerHandle timer;
			GetWorldTimerManager().SetTimer(timer, FTimerDelegate::CreateUObject(this, &ACharacter_Enemy::RemoveStatusEffect, EStatusEffect::SE_Slow), 3.0f, false);
			break;
		}
		default:
			break;
		}
	}
}

void ACharacter_Enemy::RemoveStatusEffect(EStatusEffect Effect)
{
	ActiveEffects.Remove(Effect);
	switch (Effect)
	{
	case EStatusEffect::SE_Poisen:
	{

		break;
	}
	case EStatusEffect::SE_Slow:
	{
		GetCharacterMovement()->MaxWalkSpeed = (GetCharacterMovement()->MaxWalkSpeed / 0.5f);
		break;
	}
	default:
		break;
	}
}

void ACharacter_Enemy::RemoveStatusEffect(FTimerHandle Timer)
{
	GetWorldTimerManager().ClearTimer(Timer);
	ActiveEffects.Remove(EStatusEffect::SE_Poisen);
}

// Getters/Setters ------------------------------------------------------------------------------
float ACharacter_Enemy::GetCurrentHP()
{
	return CurrentHP;
}

float ACharacter_Enemy::GetMaxHP()
{
	return MaxHP;
}

void ACharacter_Enemy::UpdateCurrentHP(float Amount)
{
	CurrentHP = CurrentHP + Amount;
}

void ACharacter_Enemy::SetMaxHP(float Amount)
{
	MaxHP = Amount;
}
