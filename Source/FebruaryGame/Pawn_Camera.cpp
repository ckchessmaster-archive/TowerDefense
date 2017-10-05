// Fill out your copyright notice in the Description page of Project Settings.

#include "FebruaryGame.h"
#include "Pawn_Camera.h"

FName APawn_Camera::MovementComponentName(TEXT("MovementComponent0"));

// Sets default values
APawn_Camera::APawn_Camera()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	Root = CreateDefaultSubobject<USphereComponent>(TEXT("Root"));
	RootComponent = Root;
	Root->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Root->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does (shouldn't be nessisary may remove later)
	CameraBoom->RelativeRotation = FRotator(-75.f, 0.f, 0.f);
	CameraBoom->TargetArmLength = 900;
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Top-down Camera"));
	TopDownCameraComponent->AttachTo(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // can probably remove later

	MovementComponent = CreateDefaultSubobject<UPawnMovementComponent, UFloatingPawnMovement>(APawn_Camera::MovementComponentName);
	MovementComponent->UpdatedComponent = Root;
}

// Called to bind functionality to input
void APawn_Camera::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	// Set gameplay key bindings for camera
	InputComponent->BindAction("ZoomIn", IE_Pressed, this, &APawn_Camera::ZoomCameraIn);
	InputComponent->BindAction("ZoomOut", IE_Pressed, this, &APawn_Camera::ZoomCameraOut);

	InputComponent->BindAxis("XMove", this, &APawn_Camera::MoveCamera);
	InputComponent->BindAxis("YMove", this, &APawn_Camera::MoveCamera);

}

void APawn_Camera::ZoomCameraIn()
{
	if (!(CameraBoom->TargetArmLength <= 600)) {
		CameraBoom->TargetArmLength = (CameraBoom->TargetArmLength - 20);
	}
}

void APawn_Camera::ZoomCameraOut()
{
	if (!(CameraBoom->TargetArmLength >= 2000)) {
		CameraBoom->TargetArmLength = (CameraBoom->TargetArmLength + 20);
	}
}

void APawn_Camera::MoveCamera(float Val)
{
	// Get mouse position
	float locationX;
	float locationY;
	GetController()->CastToPlayerController()->GetMousePosition(locationX, locationY);

	// Get viewport size
	FVector2D viewportSize = GEngine->GameViewport->Viewport->GetSizeXY();

	// Move in the direction of the cursor
	float x = 0, y = 0;
	if (locationX <= 10)
	{
		y = -1;
	}
	else if (locationX >= (viewportSize.X - 20))
	{
		y = 1;
	}
	if (locationY <= 10)
	{
		x = 1;
	}
	else if (locationY >= (viewportSize.Y - 20))
	{
		x = -1;
	}
	FVector Direction = FVector(x, y, 0);
	AddMovementInput(Direction, 10);
}
