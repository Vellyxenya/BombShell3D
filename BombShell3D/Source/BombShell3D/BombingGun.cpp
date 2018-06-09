// Fill out your copyright notice in the Description page of Project Settings.

#include "BombingGun.h"

void UBombingGun::RotateTowardsAimPoint(FRotator AimDirection) {
	SetWorldRotation(AimDirection);
}
