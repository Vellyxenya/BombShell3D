// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Robot.h"
#include "PlayerController_CPP.generated.h"

UCLASS()
class BOMBSHELL3D_API APlayerController_CPP : public APlayerController
{
	GENERATED_BODY()
	
private:
	ARobot * GetControlledRobot() const;
	
	void Tick(float DeltaTime);

	void AimTowardsCrosshair();

	bool GetSightRayHitLocation(OUT FVector & HitLocation) const;

	bool GetLookDirection(FVector2D ScreenLocation, FVector & LookDirection) const;

	bool GetLookVectorHitLocation(FVector LookDirection, OUT FVector & HitLocation) const;

	UPROPERTY(EditAnywhere)
		float CrosshairX = 0.5f;

	UPROPERTY(EditAnywhere)
		float CrosshairY = 0.4f;

	UPROPERTY(EditAnywhere)
		float LineTraceRange = 100000; //1000 m.
};
