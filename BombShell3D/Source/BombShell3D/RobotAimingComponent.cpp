// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotAimingComponent.h"
#include "BombingGun.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

URobotAimingComponent::URobotAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}

void URobotAimingComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void URobotAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void URobotAimingComponent::AimAt(FVector HitLocation, float LaunchSpeed) {
	if (!BombingGun) { return; }
	FVector OutLaunchVelocity;
	FVector StartLocation = BombingGun->GetSocketLocation(FName("Projectile")); //TODO : take into account the turret rotation as well

	if (UGameplayStatics::SuggestProjectileVelocity(
		this,
		OutLaunchVelocity,
		StartLocation,
		HitLocation,
		LaunchSpeed,
		false,
		0,
		0,
		ESuggestProjVelocityTraceOption::DoNotTrace,
		FCollisionResponseParams(),
		TArray<AActor*>(),
		false
	)) {
		auto AimDirection = OutLaunchVelocity.GetSafeNormal();
		MoveBombingGun(AimDirection);
	}
	else {
		auto Time = GetWorld()->GetTimeSeconds();
		UE_LOG(LogTemp, Warning, TEXT("%f : No aim solve found"), Time)
	}
}

void URobotAimingComponent::MoveBombingGun(FVector AimDirection) {
	auto BarrelRotator = BombingGun->GetForwardVector().Rotation();
	auto AimAsRotator = AimDirection.Rotation();
	auto DeltaRotator = AimAsRotator - BarrelRotator;
	BombingGun->RotateTowardsAimPoint(AimAsRotator); //DeltaRotator.Pitch
}

void URobotAimingComponent::SetBombingGunReference(UBombingGun* BombingGunToSet) {
	BombingGun = BombingGunToSet;
}