// Fill out your copyright notice in the Description page of Project Settings.

#include "Robot.h"
#include "RobotAimingComponent.h"
#include "Engine/World.h"
#include "BombingGun.h"
#include "Projectile.h"
#include "GameFramework/Pawn.h"
#include "PlayerController_CPP.h"
#include "Bomb.h"
#include "BombShellGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "SavingSystem.h"
#include "Soul.h"

ARobot::ARobot() {
	PrimaryActorTick.bCanEverTick = true;

	RobotAimingComponent = CreateDefaultSubobject<URobotAimingComponent>(FName("Aiming Component"));
}

void ARobot::SetBombingGunReference(UBombingGun * BombingGunToSet) {
	RobotAimingComponent->SetBombingGunReference(BombingGunToSet);
	BombingGun = BombingGunToSet;
}

void ARobot::SetSoulReference(ASoul* SoulToSet) {
	Soul = SoulToSet;
}

void ARobot::PossessSoul() {
	APlayerController_CPP* PlayerController = Cast<APlayerController_CPP>(GetController());
	PlayerController->UnPossess();
	PlayerController->Possess(Soul);
}

void ARobot::HandleInput() {
	switch (Cast<UBombShellGameInstance>(GetGameInstance())->GetGameStatus())
	{
		case GameStatus::SelectionMenu:
			//PutBomb();
			break;
		case GameStatus::Playing:
			PutBomb();
			break;
		default:
			break;
	}
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
}

void ARobot::PutBomb() {
	if (Cast<APlayerController_CPP>(GetController())->bCanPutBomb) {
		FVector AimPoint = Cast<APlayerController_CPP>(GetController())->HitLocation;
		//Alter the original aim point so that it doesn't snap inside the cube. 
		FVector AlteredAimPoint = AimPoint - (AimPoint - GetActorLocation()).GetSafeNormal() * AlterationFactor;
		GetWorld()->SpawnActor<ABomb>(Bomb_BP, AlteredAimPoint, GetActorRotation());
	}
}

void ARobot::AimAt(FVector HitLocation) {
	RobotAimingComponent->AimAt(HitLocation, LaunchSpeed);
}

void ARobot::SaveVariables() {
	PlayerName = TEXT("PlayerOne");
	USavingSystem* SaveGameInstance = Cast<USavingSystem>(UGameplayStatics::CreateSaveGameObject(USavingSystem::StaticClass()));
	SaveGameInstance->PlayerName = PlayerName;
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->SaveSlotName, SaveGameInstance->UserIndex);
}
