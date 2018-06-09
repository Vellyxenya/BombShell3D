// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "BombingGun.generated.h"

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class BOMBSHELL3D_API UBombingGun : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	void RotateTowardsAimPoint();
	
};
