// Fill out your copyright notice in the Description page of Project Settings.

#include "FebruaryGame.h"
#include "PlayerController_Main.h"
#include "GameState_Main.h"

#include "UClass_Globals.h"
#include "WorldGrid.h"

APlayerController_Main::APlayerController_Main()
{
	
}

void APlayerController_Main::SetupInputComponent()
{
	Super::SetupInputComponent();

	//InputComponent->BindAction("BuildKey", IE_Pressed, this, &APlayerController_Main::OnPressBuildMenu);
	InputComponent->BindAction("MouseClick_Left", IE_Pressed, this, &APlayerController_Main::OnMouseClick);
	InputComponent->BindAction("Cancel", IE_Pressed, this, &APlayerController_Main::Action_Cancel);
	InputComponent->BindAction("Menu", IE_Pressed, this, &APlayerController_Main::Action_Menu);
}

void APlayerController_Main::OnMouseClick()
{
	if (UnspawnedTower && UnspawnedTower->GetCanSpawn() && GetWorld()->GetGameState<AGameState_Main>()->GetMoney() >= UnspawnedTower->GetTowerCost())
	{
		UnspawnedTower->SpawnTower();
		UnspawnedTower = nullptr;
	}
}

void APlayerController_Main::Action_Cancel()
{
	if (UnspawnedTower)
	{
		UnspawnedTower->Destroy();
		UnspawnedTower = nullptr;
	}
}

// We might make this a purely blueprint function
void APlayerController_Main::Action_Menu_Implementation()
{
	
}

void APlayerController_Main::OnPressBuildMenu(FName TowerName)
{
	if (!UnspawnedTower)
	{
		FString contextString = "";
		FTowerData* towerData = GetWorld()->GetGameState<AGameState_Main>()->TowerTable->FindRow<FTowerData>(TowerName, contextString, false);

		if (towerData)
		{
			UClass* blueprint = UClass_Globals::LoadBlueprintClassFromPath(*towerData->AssetPath);

			if (blueprint)
			{
				UnspawnedTower = GetWorld()->SpawnActorDeferred<AActor_Tower>(blueprint, FTransform());

				// Set initial paramters
				
				UnspawnedTower->BaseDamage = towerData->DamageAmount;
				UnspawnedTower->Cooldown = towerData->Cooldown;
				UnspawnedTower->AttackRange->SetSphereRadius(towerData->AttackRange);
				UnspawnedTower->BonusStat = towerData->BonusStat;
				UnspawnedTower->Drain = towerData->Drain;
				UnspawnedTower->SlowPercent = towerData->SlowPercent;
				UnspawnedTower->DamageType = towerData->DamageType;
				UnspawnedTower->TowerCost = towerData->TowerCost;
				UnspawnedTower->IncomeGain = towerData->IncomeGain;

				// Finish spawning
				UnspawnedTower->FinishSpawning(FTransform());

				GetWorld()->GetGameState<AGameState_Main>()->GetWorldGrid()->AlignActorToGrid(UnspawnedTower);
			}
		}
	}
}

void APlayerController_Main::SellTower()
{

}