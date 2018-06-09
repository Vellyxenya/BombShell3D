// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RobotAimingComponent.generated.h"

class UBombingGun;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BOMBSHELL3D_API URobotAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URobotAimingComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AimAt(FVector HitLocation, float LaunchSpeed);

	void SetBombingGunReference(UBombingGun* BarrelToSet);

private:
	UBombingGun * BombingGun = nullptr;

	void MoveBombingGun(FVector AimDirection);
};
