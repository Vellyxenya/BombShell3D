// Fill out your copyright notice in the Description page of Project Settings.

#include "ExplosionRay_CPP.h"
#include "Engine/World.h"

// Sets default values
AExplosionRay_CPP::AExplosionRay_CPP()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AExplosionRay_CPP::BeginPlay()
{
	Super::BeginPlay();
}

void AExplosionRay_CPP::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

