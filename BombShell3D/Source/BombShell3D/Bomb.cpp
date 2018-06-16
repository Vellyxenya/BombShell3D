// Fill out your copyright notice in the Description page of Project Settings.

#include "Bomb.h"
#include "Engine/World.h"
#include "Effects/ExplosionRay_CPP.h"
#include "TimerManager.h"

ABomb::ABomb() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ABomb::BeginPlay() {
	Super::BeginPlay();
	FTimerHandle StartTimer;
	GetWorldTimerManager().SetTimer(StartTimer, this, &ABomb::Explode, 3);
}

void ABomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABomb::Explode() {
	UWorld* World = GetWorld();
	if (World != nullptr) {
		GetWorld()->SpawnActor<AExplosionRay_CPP>(ExplosionRay_BP, GetActorLocation(), FRotator(0, 0, 0));
		GetWorld()->SpawnActor<AExplosionRay_CPP>(ExplosionRay_BP, GetActorLocation(), FRotator(0, 90, 0));
		GetWorld()->SpawnActor<AExplosionRay_CPP>(ExplosionRay_BP, GetActorLocation(), FRotator(0, 180, 0));
		GetWorld()->SpawnActor<AExplosionRay_CPP>(ExplosionRay_BP, GetActorLocation(), FRotator(0, 270, 0));
		GetWorld()->SpawnActor<AExplosionRay_CPP>(ExplosionRay_BP, GetActorLocation(), FRotator(0, 0, 90));
		GetWorld()->SpawnActor<AExplosionRay_CPP>(ExplosionRay_BP, GetActorLocation(), FRotator(0, 0, 270));
		GetWorld()->DestroyActor(this);
	}
}
