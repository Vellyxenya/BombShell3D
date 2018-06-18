// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerController_CPP.h"
#include "Robot.h"
#include "Engine/World.h"
#include "ConstructorHelpers.h"
#include "UserWidget.h"
#include "BombShellGameInstance.h"
#include "UObject/UObjectIterator.h"
#include "CubeArrow_CPP.h"

#define OUT

void APlayerController_CPP::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	AimTowardsCrosshair();
	FString name = GetWorld()->GetMapName();
	//UE_LOG(LogTemp, Warning, TEXT("Level name : %s"), *name);

	if (GetDistanceToAimPoint() <= PutBombRange) {
		bCanPutBomb = true;
	} else {
		bCanPutBomb = true;//false; just for testing
	}
}

ARobot * APlayerController_CPP::GetControlledRobot() const {
	return Cast<ARobot>(GetPawn());
}

void APlayerController_CPP::AimTowardsCrosshair() {
	if (!GetControlledRobot()) { return; }

	//HitLocation is an Out parameter
	GetSightRayHitLocation(HitLocation);
	GetControlledRobot()->AimAt(HitLocation);
}

bool APlayerController_CPP::GetSightRayHitLocation(OUT FVector& HitLocation) {
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);
	FVector2D ScreenLocation = FVector2D(CrosshairX*ViewportSizeX, CrosshairY*ViewportSizeY);
	FVector LookDirection;

	if (GetLookDirection(ScreenLocation, LookDirection)) {
		GetLookVectorHitLocation(LookDirection, HitLocation);
	}
	return true;
}

float APlayerController_CPP::GetDistanceToAimPoint() {
	if (GetControlledRobot() != nullptr) {
		FVector ActorLocation = GetControlledRobot()->GetActorLocation();
		FVector HitLocation;
		APlayerController_CPP::GetSightRayHitLocation(HitLocation);
		FVector DistanceVector = HitLocation - ActorLocation;
		return DistanceVector.Size();
	}
	return 9999999;
}

bool APlayerController_CPP::GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const {
	FVector CameraWorldLocation; //To be discarded
	return DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, OUT CameraWorldLocation, OUT LookDirection);
}

bool APlayerController_CPP::GetLookVectorHitLocation(FVector LookDirection, OUT FVector& HitLocation) {
	FHitResult HitResult;
	auto StartLocation = PlayerCameraManager->GetCameraLocation();
	auto EndLocation = StartLocation + LookDirection * LineTraceRange;

	if (GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		ECollisionChannel::ECC_Visibility)) {
		HitLocation = HitResult.Location;
		//UE_LOG(LogTemp, Warning, TEXT("HitResult : %s"), *HitResult.GetComponent()->GetClass()->GetName());
		Arrow = Cast<UCubeArrow_CPP>(HitResult.GetComponent());
		if (Arrow != nullptr) {
			if (Arrow->ThisName == FString("RightArrow")) {
				UE_LOG(LogTemp, Warning, TEXT("RIGHT!"));
			}
			else if (Arrow->ThisName == FString("LeftArrow")) {
				UE_LOG(LogTemp, Warning, TEXT("LEFT!"));
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("NOTHING"));
			}
		}
		return true;
	}
	HitLocation = FVector(0);
	return false;
}