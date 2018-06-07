// Fill out your copyright notice in the Description page of Project Settings.

#include "DestructibleCube_CPP.h"


// Sets default values
ADestructibleCube_CPP::ADestructibleCube_CPP()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADestructibleCube_CPP::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ADestructibleCube_CPP::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

