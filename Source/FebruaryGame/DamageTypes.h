// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UClass_Globals.h"

#include "DamageTypes.generated.h"

USTRUCT()
struct FEBRUARYGAME_API FCustomDamageEvent : public FDamageEvent
{
	GENERATED_USTRUCT_BODY()

	EDamageType DamageType;
};