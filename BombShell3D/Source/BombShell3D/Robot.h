// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BombShell3DCharacter.h"
#include "Robot.generated.h"

class URobotAimingComponent;
class UBombingGun;
class ASoul;

UCLASS()
class BOMBSHELL3D_API ARobot : public ABombShell3DCharacter
{
	GENERATED_BODY()
	
public:
	ARobot();

	void AimAt(FVector HitLocation);

	UFUNCTION(BlueprintCallable, Category = Setup)
		void SetBombingGunReference(UBombingGun* BombingGunToSet);
	UFUNCTION(BlueprintCallable, Category = Setup)
		void SetSoulReference(ASoul* Soul);

	UFUNCTION(BlueprintCallable, Category = Input)
		void HandleInput();

	void PutBomb();
	void Fire();

	void SaveVariables();

	UFUNCTION(BlueprintCallable)
		void PossessSoul();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ASoul* Soul = nullptr;

protected:
	URobotAimingComponent * RobotAimingComponent = nullptr;

private:
	FString PlayerName;

	double LastFireTime = 0;

	UPROPERTY(EditDefaultsOnly, Category = Firing)
		float ReloadTimeInSeconds = 2;

	UPROPERTY(EditDefaultsOnly, Category = Firing)
		float LaunchSpeed = 3000; // 30 m/s

	UPROPERTY(EditDefaultsOnly, Category = Setup)
		UClass* Projectile_BP;

	UPROPERTY(EditDefaultsOnly, Category = Setup)
		UClass* Bomb_BP;

	UBombingGun* BombingGun = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = snapping)
		double AlterationFactor = 1;
};
