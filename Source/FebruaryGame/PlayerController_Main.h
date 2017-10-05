// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "Actor_Tower.h"

#include "PlayerController_Main.generated.h"


UCLASS()
class FEBRUARYGAME_API APlayerController_Main : public APlayerController
{
	GENERATED_BODY()

	virtual void SetupInputComponent() override;

	UFUNCTION()
	void OnMouseClick();
	UFUNCTION()
	void Action_Cancel();

	TSubclassOf<class AActor_Tower> TowerToSpawn;
	AActor_Tower* UnspawnedTower;

	// Is the mouse cursor over an unit or tower
	bool bIsCursorOverValidObject;

protected:

	UFUNCTION(BlueprintNativeEvent, Category = StateChange)
	void Action_Menu();
	void Action_Menu_Implementation();

public:

	APlayerController_Main();

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void OnPressBuildMenu(FName TowerName);

	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void SellTower();
};
