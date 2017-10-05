// Fill out your copyright notice in the Description page of Project Settings.

#include "FebruaryGame.h"
#include "Actor_Tower.h"

#include "GameState_Main.h"
#include "Character_Enemy.h"
#include "DamageTypes.h"

#include "WorldGrid.h"

// Sets default values
AActor_Tower::AActor_Tower()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;

	AttackRange = CreateDefaultSubobject<USphereComponent>(TEXT("AttackRange"));
	AttackRange->AttachTo(StaticMesh);
	AttackRange->OnComponentBeginOverlap.AddDynamic(this, &AActor_Tower::OnBeginAttackOverlap);
	AttackRange->OnComponentEndOverlap.AddDynamic(this, &AActor_Tower::OnEndAttackOverlap);
}

// Called when the game starts or when spawned
void AActor_Tower::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(AttackTimerHandle, FTimerDelegate::CreateUObject(this, &AActor_Tower::Attack), Cooldown, true, 0.0f);
	GetWorldTimerManager().PauseTimer(AttackTimerHandle);
}

// Called every frame
void AActor_Tower::Tick(float DeltaTime)
{
	Super::Tick( DeltaTime );

	// If the tower hasn't been placed yet follow the cursor
	if (!IsSpawned)
	{
		FHitResult hit;
		bool isValid = GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1, false, hit);
		
		if (isValid) // Make sure we have a mouse position
		{
			// Move the actor based on the grid
			UWorldGrid* grid = GetWorld()->GetGameState<AGameState_Main>()->GetWorldGrid();
			if (grid)
			{
				FVector moveToLocation = grid->GetClosestGridCoordinates(hit.Location, GridHeight, GridWidth);
				SetActorLocation(moveToLocation);

				if (grid->IsGridAreaFree(GetActorLocation(), GridHeight, GridWidth) && (GetWorld()->GetGameState<AGameState_Main>()->GetMoney() >= TowerCost)) // Check if we can spawn here
				{
					SetCanSpawn(true);
				}
				else
				{
					SetCanSpawn(false);
				}
			}
		}
	}
}

void AActor_Tower::OnBeginAttackOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsSpawned && !Target && Cast<ACharacter_Enemy>(OtherActor))
	{
		Target = OtherActor;
		GetWorldTimerManager().UnPauseTimer(AttackTimerHandle);
	}
}

void AActor_Tower::OnEndAttackOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IsSpawned && OtherActor && OtherActor == Target)
	{
		GetWorldTimerManager().PauseTimer(AttackTimerHandle);
		Target = nullptr;

		// Set the next actor that is in our range as the target, if none exists than it will remain null
		TArray<AActor*> overallpingActors;
		AttackRange->GetOverlappingActors(overallpingActors);
		for (AActor* actor : overallpingActors)
		{
			if (Cast<ACharacter_Enemy>(actor))
			{
				Target = actor;
				NewTargetNotify();
				GetWorldTimerManager().UnPauseTimer(AttackTimerHandle);
				break;
			}
		}
	}
}

void AActor_Tower::Attack_Implementation()
{
	bWeaponFired = false;
	if (Target)
	{
		FCustomDamageEvent damageEvent;
		damageEvent.DamageType = DamageType;

		switch (AttackType)
		{
		case EAttackType::AT_Beam:
		{
			damageEvent.DamageType = DamageType;
			Target->TakeDamage(BaseDamage, damageEvent, nullptr, this);
			break;
		}
		case EAttackType::AT_AOESphere:
			break;
		case EAttackType::AT_AOELine:
			break;
		default:
			break;
		}

		// Add any effects
		for (EStatusEffect Effect : StatusEffects)
		{
			Cast<ACharacter_Enemy>(Target)->AddStatusEffect(Effect);
		}
	}
	bWeaponFired = true;
}

void AActor_Tower::SpawnTower_Implementation()
{
	// Gamestate reference
	AGameState_Main* gamestate = GetWorld()->GetGameState<AGameState_Main>();

	// Set cells as occupied
	gamestate->GetWorldGrid()->SetCellsAsOccupied(this, true, GridHeight, GridWidth);
	gamestate->UpdateMoney(-1 * TowerCost);
	gamestate->UpdateIncome(IncomeGain);

	IsSpawned = true;
}

void AActor_Tower::SellTower()
{
	GetWorld()->GetGameState<AGameState_Main>()->UpdateMoney(TowerCost / 2);
	GetWorld()->GetGameState<AGameState_Main>()->UpdateIncome(-1 * IncomeGain);
	Destroy();
}

void AActor_Tower::SetCanSpawn_Implementation(bool AbleToSpawn)
{
	CanSpawn = AbleToSpawn;
}

int32 AActor_Tower::GetTowerCost()
{
	return TowerCost;
}

bool AActor_Tower::GetIsSpawned()
{
	return IsSpawned;
}

bool AActor_Tower::GetCanSpawn()
{
	return CanSpawn;
}