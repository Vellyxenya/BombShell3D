// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UProjectileMovementComponent;

UCLASS()
class BOMBSHELL3D_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectile();
	void GetFired(float Speed);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
private:
	UProjectileMovementComponent * ProjectileMovementComponent = nullptr;
};
