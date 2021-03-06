// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Robot.h"
#include "PlayerController_CPP.generated.h"

class UCubeArrow_CPP;

UCLASS()
class BOMBSHELL3D_API APlayerController_CPP : public APlayerController
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bCanPutBomb = false;

	FVector HitLocation;
	FHitResult HitResult;

private:
	ARobot * GetControlledRobot() const;

	UCubeArrow_CPP* Arrow = nullptr;
	
	void Tick(float DeltaTime);

	void AimTowardsCrosshair();

	bool GetSightRayHitLocation(OUT FVector & HitLocation, OUT FHitResult& HitResult);

	bool GetLookDirection(FVector2D ScreenLocation, FVector & LookDirection) const;

	bool GetLookVectorHitLocation(FVector LookDirection, OUT FVector & HitLocation, OUT FHitResult& HitResult);

	float GetDistanceToAimPoint();

	UFUNCTION(BlueprintCallable)
		void HandleInput();

	UPROPERTY(EditAnywhere)
		float CrosshairX = 0.5f;

	UPROPERTY(EditAnywhere)
		float CrosshairY = 0.4f;

	UPROPERTY(EditAnywhere)
		float LineTraceRange = 100000; //1000 m.

	UPROPERTY(EditAnywhere)
		float PutBombRange = 1000; //10 m.
};
