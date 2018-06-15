// Fill out your copyright notice in the Description page of Project Settings.

#include "Robot.h"
#include "RobotAimingComponent.h"
#include "Engine/World.h"
#include "BombingGun.h"
#include "Projectile.h"
#include "GameFramework/Pawn.h"
#include "PlayerController_CPP.h"
#include "Bomb.h"

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
		UE_LOG(LogTemp, Warning, TEXT("ALSKJFAéLSJFDéAJFLSAJDFAS"));
		if (!ensure(Projectile != nullptr)) return;
		Projectile->GetFired(LaunchSpeed);
		LastFireTime = FPlatformTime::Seconds();
	}
	//PutBomb();
}

void ARobot::PutBomb() {
	//TODO : this function puts bomb if the crosshair is aiming at a certain range.
	//No projectiles involved

	if (Cast<APlayerController_CPP>(GetController())->bCanPutBomb) {
		FVector AimPoint = Cast<APlayerController_CPP>(GetController())->HitLocation;
		FRotator Rotation = FRotator();
		GetWorld()->SpawnActor<ABomb>(Bomb_BP, AimPoint, Rotation);
	}
}

void ARobot::AimAt(FVector HitLocation) {
	RobotAimingComponent->AimAt(HitLocation, LaunchSpeed);
}



