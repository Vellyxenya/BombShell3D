// Fill out your copyright notice in the Description page of Project Settings.

#include "Bomb.h"
#include "Engine/World.h"
#include "Effects/ExplosionRay_CPP.h"
#include "TimerManager.h"

ABomb::ABomb() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	UE_LOG(LogTemp, Warning, TEXT("Location : %s"), *Location.ToString());
}

void ABomb::BeginPlay() {
	Super::BeginPlay();
	
	FTimerHandle LockTimer;
	FTimerHandle ExplosionTimer;
	GetWorldTimerManager().SetTimer(LockTimer, this, &ABomb::Lock, 3);
	GetWorldTimerManager().SetTimer(ExplosionTimer, this, &ABomb::CreateExplosions, 5);
	OnActorBeginOverlap.AddDynamic(this, &ABomb::OnOverlap);
}

void ABomb::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
}

void ABomb::Lock() {
	OnExplode.Broadcast();
	UE_LOG(LogTemp, Warning, TEXT("Broadcasting"));
}

void ABomb::OnOverlap(AActor* ThisActor, AActor* OtherActor) {
	UE_LOG(LogTemp, Warning, TEXT("Overlapping"));
	if (Cast<AExplosionRay_CPP>(OtherActor)) {
		GetWorld()->DestroyActor(this); //It is critical that this line comes before CreateExplosions() otherwise it crashes for unknown reason
		//with shitty log error, and you end up debugging for a few hours.
		GetWorld()->DestroyActor(OtherActor);
		CreateExplosions(); //this makes the game crash if the actor isn't destroyed beforehand.
		UE_LOG(LogTemp, Warning, TEXT("Cast Success"));
	} else {
		UE_LOG(LogTemp, Warning, TEXT("Cast Failed"));
	}
}

void ABomb::CreateExplosions() {
	UWorld* World = GetWorld();
	if (World != nullptr) {
		//GetWorld()->SpawnActor<AExplosionRay_CPP>(ExplosionRay_BP, GetActorLocation(), FRotator(Pitch, Roll, Yaw));
		World->SpawnActor<AExplosionRay_CPP>(ExplosionRay_BP, GetActorLocation(), FRotator(0, 0, 0));
		World->SpawnActor<AExplosionRay_CPP>(ExplosionRay_BP, GetActorLocation(), FRotator(0, 90, 0));
		World->SpawnActor<AExplosionRay_CPP>(ExplosionRay_BP, GetActorLocation(), FRotator(0, 180, 0));
		World->SpawnActor<AExplosionRay_CPP>(ExplosionRay_BP, GetActorLocation(), FRotator(0, 270, 0));
		World->SpawnActor<AExplosionRay_CPP>(ExplosionRay_BP, GetActorLocation(), FRotator(90, 0, 0));
		World->SpawnActor<AExplosionRay_CPP>(ExplosionRay_BP, GetActorLocation(), FRotator(270, 0, 0));
		World->DestroyActor(this);
	}
}
