// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "Pawn_Camera.generated.h"

class UFloatingPawnMovement;

UCLASS()
class FEBRUARYGAME_API APawn_Camera : public APawn
{
	GENERATED_BODY()

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	UFUNCTION()
	void ZoomCameraIn();
	UFUNCTION()
	void ZoomCameraOut();
	UFUNCTION()
	void MoveCamera(float Val);

	/** DefaultPawn movement component */
	UPROPERTY(Category = Pawn, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UPawnMovementComponent* MovementComponent;

protected:

	

public:
	// Sets default values for this pawn's properties
	APawn_Camera();

	/** Name of the MovementComponent.  Use this name if you want to use a different class (with ObjectInitializer.SetDefaultSubobjectClass). */
	static FName MovementComponentName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	USphereComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* TopDownCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	USpringArmComponent* CameraBoom;
};
