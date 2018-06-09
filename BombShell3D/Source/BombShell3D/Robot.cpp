// Fill out your copyright notice in the Description page of Project Settings.

#include "Robot.h"
#include "RobotAimingComponent.h"
#include "Engine/World.h"
#include "BombingGun.h"
#include "Projectile.h"

ARobot::ARobot() {
	PrimaryActorTick.bCanEverTick = true;

	RobotAimingComponent = CreateDefaultSubobject<URobotAimingComponent>(FName("Aiming Component"));
}

void ARobot::SetBombingGunReference(UBombingGun * BombingGunToSet) {
	RobotAimingComponent->SetBombingGunReference(BombingGunToSet);
	BombingGun = BombingGunToSet;
}

void ARobot::Fire() {
	bool isReloaded = (FPlatformTime::Seconds() - LastFireTime) > ReloadTimeInSeconds;

	if (BombingGun && isReloaded) {
		auto Projectile = GetWorld()->SpawnActor<AProjectile>(
			Projectile_BP,
			BombingGun->GetSocketLocation(FName("ProjectileLaunchPoint")),
			BombingGun->GetSocketRotation(FName("ProjectileLaunchPoint"))
			);
		if (!ensure(Projectile != nullptr)) return;
		Projectile->GetFired(LaunchSpeed);
		LastFireTime = FPlatformTime::Seconds();
	}
}

void ARobot::AimAt(FVector HitLocation) {
	RobotAimingComponent->AimAt(HitLocation, LaunchSpeed);
}



